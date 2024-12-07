#include "s21_string.h"

// Структура для хранения флагов форматирования
typedef struct Flags {
    bool alignment;  // выравнивание по левому краю
    bool sign;       // выводить знак +
    bool space;  // Если знак не будет выведен, перед значением вставляется пробел
    bool sharp;  // префиксы для oct/hex ...
    bool zero;   // ставить 0 перед числом
    bool is_negative;

    bool is_up;  // заглавные буквы

    s21_size_t width;  // минимальное количество печатаемых символов (ширина)
    int accuracy;      // точность для double, ширина для int

    char size;  // длина h, l, L
    bool is_g;
} Flags;

void s21_int_to_string(char *result, long long int num, int num_sys, Flags *flags);
void s21_uint_to_str(char *result, unsigned long long int num, int num_sys, Flags *flags);
static void s21_sprintf_flags(char c, Flags *flags);
void s21_str_to_str(char *result, char *str, Flags *flags);
static void s21_flags_int(char *result, char *str, char *prefix, Flags *flags);
void process_specifier(char **str, char specifier, va_list factor, Flags flags, char *start_str);

// Функция для получения целочисленного аргумента в зависимости от размера
long long int get_arg_int(va_list factor, Flags flags) {
    return (flags.size == 'l') ? va_arg(factor, long long int) : va_arg(factor, int);
}

// Функция для получения беззнакового целочисленного аргумента в зависимости от размера
unsigned long long int get_arg_uint(va_list factor, Flags flags) {
    return (flags.size == 'l') ? va_arg(factor, unsigned long long int) : va_arg(factor, unsigned int);
}

// Функция для получения аргумента с плавающей точкой в зависимости от размера
long double get_arg_float(va_list factor, Flags flags) {
    return (flags.size == 'L') ? va_arg(factor, long double) : va_arg(factor, double);
}

// Функция для преобразования целого числа в строку
void s21_int_to_string(char *result, long long int num, int num_sys, Flags *flags) {
    if (result != S21_NULL && num_sys != 0) {
        char str[50] = "";
        char prefix[3] = "";
        int i = 0;

        if (num == 0 && flags->accuracy != 0) {
            str[0] = '0';
            i++;
        }

        while (num != 0) {
            int rem_num = abs((int)(num % num_sys));
            if (rem_num < 10) {
                str[i] = rem_num + '0';
            } else {
                str[i] = rem_num - 10 + 'a';
            }
            num /= num_sys;
            i++;
        }

        for (int j = 0; j < i / 2; j++) {
            char tmp = str[j];
            str[j] = str[i - j - 1];
            str[i - j - 1] = tmp;
        }

        if (num_sys == 8 || num_sys == 16) {
            if (flags->sharp && num_sys == 8) {
                strncat(prefix, "0", 1);
                if (flags->accuracy > 0) {
                    flags->accuracy--;
                }
            } else if (flags->sharp) {
                if (flags->accuracy > 1) {
                    flags->accuracy -= 2;
                }
                strncat(prefix, "0x", 2);
            }
        } else if (flags->is_negative) {
            strncat(prefix, "-", 1);
        } else if (flags->sign) {
            strncat(prefix, "+", 1);
        } else if (flags->space) {
            strncat(prefix, " ", 1);
        }

        s21_flags_int(result, str, prefix, flags);
    }
}

// Функция для обнуления флагов
static void s21_zero_flags(Flags *flags) {
    memset(flags, 0, sizeof(Flags));
    flags->accuracy = -1;
}

// Функция для обработки флагов форматирования
static void s21_sprintf_flags(char c, Flags *flags) {
    switch (c) {
        case '-':
            flags->alignment = true;
            break;
        case '+':
            flags->sign = true;
            break;
        case ' ':
            flags->space = true;
            break;
        case '#':
            flags->sharp = true;
            break;
        case '0':
            flags->zero = true;
            break;
    }
    if (flags->alignment && flags->zero) {
        flags->zero = false;
    }
    if (flags->sign && flags->space) {
        flags->space = false;
    }
}

// Функция для обработки ошибок форматирования
static void s21_errors_sprintf(char *str, Flags flags) {
    strncat(str, "%", 1);
    if (flags.sharp) {
        strncat(str, "#", 1);
    }
    if (flags.sign) {
        strncat(str, "+", 1);
    }
    if (flags.alignment) {
        strncat(str, "-", 1);
    }
    if (flags.width) {
        Flags fl = {0};
        s21_uint_to_str(str, flags.width, 10, &fl);
    }
    if (flags.accuracy) {
        Flags fl = {0};
        strncat(str, ".", 1);
        s21_uint_to_str(str, flags.accuracy, 10, &fl);
    }
}

// Функция для чтения целого числа из строки
static int s21_sprintf_read_int(const char *str, int *i) {
    unsigned long long int ret_int = 0;
    while (isdigit(str[*i])) {
        ret_int *= 10;
        ret_int += str[*i] - '0';
        (*i)++;
        if (ret_int > INT_MAX) {
            ret_int = (unsigned long long int)INT_MAX + 1;
        }
    }
    if (ret_int > INT_MAX) {
        ret_int = -1;
    }
    return ret_int;
}

// Функция для преобразования символа в строку
void s21_char_to_str(char **result, char c) {
    if (*result != S21_NULL) {
        char str[2] = "";
        str[0] = c;
        str[1] = '\0';
        strncat(*result, str, 1);
        if (c == '\0') {
            *result += s21_strlen(*result) + 1;
        }
    }
}

// Функция для обработки флагов целочисленного форматирования
static void s21_flags_int(char *result, char *str, char *prefix, Flags *flags) {
    if (result != S21_NULL && str != S21_NULL && prefix != S21_NULL) {
        int count_space = 0;
        int count_zero = 0;
        if (!flags->alignment && flags->accuracy == -1 &&
            flags->width > s21_strlen(prefix) + s21_strlen(str) && flags->zero) {
            flags->accuracy = flags->width - s21_strlen(prefix);
        }
        count_zero = flags->accuracy - s21_strlen(str);
        if (count_zero > 0) {
            count_space =
                flags->width - count_zero - s21_strlen(str) - s21_strlen(prefix);
        } else {
            count_space = flags->width - s21_strlen(str) - s21_strlen(prefix);
        }

        if (!flags->alignment) {
            for (int i = 0; i < count_space; i++) {
                strncat(result, " ", 1);
            }
        }
        strncat(result, prefix, s21_strlen(prefix));
        for (int i = 0; i < count_zero; i++) {
            strncat(result, "0", 1);
        }
        strncat(result, str, s21_strlen(str));
        if (flags->alignment) {
            for (int i = 0; i < count_space; i++) {
                strncat(result, " ", 1);
            }
        }
    }
}

// Функция для преобразования беззнакового целого числа в строку
void s21_uint_to_str(char *result, unsigned long long int num, int num_sys,
                     Flags *flags) {
    s21_int_to_string(result, num, num_sys, flags);
}

// Функция для обработки флагов форматирования чисел с плавающей точкой
static void s21_flags_float(char *result, char *str, char *prefix,
                            Flags *flags) {
    if (result != S21_NULL && str != S21_NULL && prefix != S21_NULL) {
        int count_zero = 0;
        if (flags->zero) {
            count_zero = flags->width - s21_strlen(str) - s21_strlen(prefix);
        }
        int count_space =
            flags->width - s21_strlen(str) - s21_strlen(prefix) - count_zero;

        if (!flags->alignment) {
            for (int i = 0; i < count_space; i++) {
                strncat(result, " ", 1);
            }
        }
        strncat(result, prefix, s21_strlen(prefix));
        for (int i = 0; i < count_zero; i++) {
            strncat(result, "0", 1);
        }
        strncat(result, str, s21_strlen(str));
        if (flags->alignment) {
            for (int i = 0; i < count_space; i++) {
                strncat(result, " ", 1);
            }
        }
    }
}

// Функция для преобразования числа с плавающей точкой в строку
void s21_float_to_str(char *result, long double num, Flags flags) {
    if (result != S21_NULL && !isinf((double)num) && !isnan((double)num)) {
        Flags int_flags;
        s21_zero_flags(&int_flags);
        char prefix[2] = "";
        char float_str[10000] = "0";
        if (flags.sign && num >= 0) {
            strncat(prefix, "+", 1);
        } else if (num < 0) {
            num *= -1;
            strncat(prefix, "-", 1);
        } else if (flags.space) {
            strncat(prefix, " ", 1);
        }
        int count_div = 0;
        while (num >= 1.0) {
            num /= 10;
            count_div++;
        }
        if (count_div == 0) {
            strncat(float_str, "0", 1);
        }
        while (count_div > 0) {
            num = (num - (int)num) * 10;
            s21_int_to_string(float_str, (int)num, 10, &int_flags);
            count_div--;
        }
        if (flags.is_g) {
            if (float_str[1] != '0' && s21_strlen(float_str) != 2) {
                flags.accuracy -= s21_strlen(float_str) - 1;
            }
        }
        if (flags.sharp || flags.accuracy != 0) {
            strncat(float_str, ".", 1);
        }
        while (flags.accuracy > 0) {
            num = (num - (int)num) * 10;
            s21_int_to_string(float_str, (int)num, 10, &int_flags);
            flags.accuracy--;
        }

        int round = (int)((num - (int)num) * 10);
        if (round >= 5) {
            round = 1;
            int pos = s21_strlen(float_str) - 1;
            while (pos >= 0 && round != 0) {
                if (float_str[pos] != '.') {
                    float_str[pos] += round;
                    if (float_str[pos] > '9') {
                        float_str[pos] = '0';
                        round = 1;
                    } else {
                        round = 0;
                    }
                }
                pos--;
            }
        }
        if (flags.is_g && strchr(float_str, '.') != S21_NULL && !flags.sharp) {
            s21_size_t i = s21_strlen(float_str) - 1;
            while (float_str[i] == '0') {
                float_str[i] = '\0';
                i--;
            }
            if (float_str[i] == '.') {
                float_str[i] = '\0';
            }
        }
        if (float_str[0] == '1') {
            s21_flags_float(result, float_str, prefix, &flags);
        } else {
            s21_flags_float(result, float_str + 1, prefix, &flags);
        }
    } else if (result != S21_NULL) {
        if (isinf((double)num)) {
            if (num > 0) {
                s21_str_to_str(result, "inf", &flags);
            } else {
                s21_str_to_str(result, "-inf", &flags);
            }
        } else {
            s21_str_to_str(result, "nan", &flags);
        }
    }
}

// Функция для преобразования числа с плавающей точкой в экспоненциальную форму
void s21_notat_float_to_str(char *result, long double num, Flags flags) {
    if (result != S21_NULL && !isinf((double)num) && !isnan((double)num)) {
        char prefix[2] = "";
        char float_str[10000] = "";
        if (flags.sign && num >= 0) {
            strncat(prefix, "+", 1);
        } else if (num < 0) {
            num *= -1;
            strncat(prefix, "-", 1);
        } else if (flags.space) {
            strncat(prefix, " ", 1);
        }
        int exp = 0;
        if (num >= 1.0) {
            while (num >= 10.0) {
                num /= 10;
                exp++;
            }
        } else {
            while (num < 1.0 && num != 0) {
                num *= 10;
                exp--;
            }
        }
        Flags float_flags;
        s21_zero_flags(&float_flags);
        if (flags.is_g) {
            flags.accuracy--;
        }
        float_flags.accuracy = flags.accuracy;
        float_flags.sharp = flags.sharp;
        s21_float_to_str(float_str, num, float_flags);
        if (float_str[2] == '.') {
            char x = float_str[1];
            float_str[1] = float_str[2];
            float_str[2] = x;
            exp++;
            float_str[s21_strlen(float_str) - 1] = '\0';
        }
        if (flags.is_g && strchr(float_str, '.') != S21_NULL && !flags.sharp) {
            s21_size_t i = s21_strlen(float_str) - 1;
            while (float_str[i] == '0') {
                float_str[i] = '\0';
                i--;
            }
            if (float_str[i] == '.') {
                float_str[i] = '\0';
            }
        }
        strncat(float_str, "e", 1);
        if (exp < 0) {
            strncat(float_str, "-", 1);
            exp *= -1;
        } else {
            strncat(float_str, "+", 1);
        }
        if (exp < 10) {
            strncat(float_str, "0", 1);
        }
        float_flags.accuracy = -1;
        s21_int_to_string(float_str, exp, 10, &float_flags);
        s21_flags_float(result, float_str, prefix, &flags);
    } else if (result != S21_NULL) {
        if (isinf((double)num)) {
            if (num > 0) {
                s21_str_to_str(result, "inf", &flags);
            } else {
                s21_str_to_str(result, "-inf", &flags);
            }
        } else {
            s21_str_to_str(result, "nan", &flags);
        }
    }
}

// Функция для подсчета значащих цифр в числе с плавающей точкой
static s21_size_t s21_significant_digit_float(char *res) {
    int count = 0;
    if (res != S21_NULL) {
        s21_size_t i = 0;
        while ((!isdigit(res[i]) || res[i] == '0') && i < s21_strlen(res)) {
            i++;
        }
        while ((isdigit(res[i]) || res[i] == '.') && i < s21_strlen(res)) {
            if (res[i] != '.') {
                count++;
            }
            i++;
        }
    }
    return count;
}

// Функция для преобразования числа с плавающей точкой в формат g или G
void s21_g_float_to_str(char *result, long double num, Flags *flags) {
    if (result != S21_NULL) {
        s21_size_t len0 = s21_strlen(result);
        s21_notat_float_to_str(result, num, *flags);
        s21_size_t len1 = s21_significant_digit_float(result + len0);
        result[len0] = '\0';
        s21_float_to_str(result, num, *flags);
        s21_size_t len2 = s21_significant_digit_float(result + len0);
        if (len1 < len2) {
            result[len0] = '\0';
            s21_notat_float_to_str(result, num, *flags);
        }
    }
}

// Функция для преобразования строки в строку с учетом флагов форматирования
void s21_str_to_str(char *result, char *str, Flags *flags) {
    if (result != S21_NULL && str != S21_NULL) {
        int count = (int)s21_strlen(str);
        if (flags->accuracy >= 0) {
            count = (int)s21_strlen(str) < flags->accuracy ? (int)s21_strlen(str)
                                                           : flags->accuracy;
        }
        if ((int)flags->width > count && !flags->alignment) {
            for (s21_size_t i = 0; i < flags->width - count; i++) {
                strncat(result, " ", 1);
            }
        }

        strncat(result, str, count);

        if ((int)flags->width > count && flags->alignment) {
            for (s21_size_t i = 0; i < flags->width - count; i++) {
                strncat(result, " ", 1);
            }
        }
    } else if (str == S21_NULL) {
        if (flags->accuracy >= 6 || flags->accuracy == -1) {
            s21_str_to_str(result, "(null)", flags);
        } else {
            s21_str_to_str(result, "", flags);
        }
    }
}

// Функция для подсчета длины строки wchar_t
static s21_size_t s21_wcslen(wchar_t *str) {
    s21_size_t len = 0;
    if (str != NULL) {
        for (; str[len]; len++)
            ;
    }
    return len;
}

// Функция для преобразования указателя в строку
void s21_ptr_to_str(char *result, void *ptr, Flags *flags) {
    if (result != S21_NULL) {
        if (ptr == S21_NULL) {
            strncat(result, "(nil)", 5);
        } else {
            flags->sharp = true;
            if (flags->sign) {
                strncat(result, "+", 1);
                if (flags->width) {
                    flags->width--;
                }
            }
            if (flags->accuracy) {
                flags->accuracy += 2;
            }
            s21_uint_to_str(result, (long long int)ptr, 16, flags);
        }
    }
}

// Функция для преобразования строки в заглавные буквы
static void s21_upper_str(char *str) {
    if (str != NULL) {
        s21_size_t len = s21_strlen(str);
        for (s21_size_t i = 0; i < len; i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 32;
            }
        }
    }
}



// Основная функция для форматирования строки
// int s21_sprintf(char *str, const char *format, ...) {
//     s21_size_t len = 0;
//     if (str != S21_NULL && format != S21_NULL) {
//         str[0] = '\0';
//         char *start_str = str;
//         va_list factor;            // указатель va_list
//         va_start(factor, format);  // устанавливаем указатель
//         int i = 0;
//         int state = 0;
//         Flags flags;
//         s21_zero_flags(&flags);

//         // %[флаги][ширина][.точность][длина]спецификатор.
//         while (format[i] != '\0') {
//             if (format[i] == '%' && state == 0) {
//                 s21_zero_flags(&flags);
//                 state = 1;
//             } else if (is_flag(format[i]) && state > 0) {
//                 s21_sprintf_flags(format[i], &flags);
//             } else if ((is_digit(format[i]) || format[i] == '*') && state == 1) {
//                 if (is_digit(format[i])) {
//                     flags.width = s21_sprintf_read_int(format, &i);
//                     i--;
//                 } else {
//                     flags.width = va_arg(factor, int);
//                 }
//                 state = 2;
//             } else if (format[i] == '.' && state > 0 && state < 3) {
//                 i++;
//                 if (format[i] == '*') {
//                     flags.accuracy = va_arg(factor, int);
//                 } else {
//                     flags.accuracy = s21_sprintf_read_int(format, &i);
//                     i--;
//                 }
//                 state = 3;
//             } else if (is_length(format[i]) && state > 0 && state < 4) {
//                 flags.size = format[i];
//                 state = 4;
//             } else if (is_specifier(format[i]) && state > 0) {
//                 if (s21_strchr("eEfgG", format[i]) && flags.accuracy == -1) {
//                     flags.accuracy = 6;
//                 }
//                 char *ptr = str + s21_strlen(str);
//                 switch (format[i]) {
//                     case '%':
//                         s21_char_to_str(&str, '%');
//                         break;
//                     case 'c':
//                         s21_char_to_str(&str, va_arg(factor, int));
//                         break;
//                     case 's':
//                         if (flags.size == 'l') {
//                             wchar_t *wstr = va_arg(factor, wchar_t *);
//                             if (wstr != NULL) {
//                                 int buf_size = s21_wcslen(wstr);
//                                 char *buf = malloc(sizeof(char) * (buf_size + 1));
//                                 wcstombs(buf, wstr, buf_size + 1);
//                                 s21_str_to_str(str, buf, &flags);
//                                 free(buf);
//                             } else {
//                                 s21_str_to_str(str, NULL, &flags);
//                             }
//                         } else {
//                             s21_str_to_str(str, va_arg(factor, char *), &flags);
//                         }
//                         break;
//                     case 'd':
//                     case 'i':
//                         s21_int_to_string(str, get_arg_int(factor, flags), 10, &flags);
//                         break;
//                     case 'o':
//                         s21_int_to_string(str, get_arg_uint(factor, flags), 8, &flags);
//                         break;
//                     case 'X':
//                         flags.is_up = true;
//                         s21_int_to_string(str, get_arg_uint(factor, flags), 16, &flags);
//                         break;
//                     case 'x':
//                         s21_int_to_string(str, get_arg_uint(factor, flags), 16, &flags);
//                         break;
//                     case 'u':
//                         flags.sign = false;
//                         s21_int_to_string(str, get_arg_uint(factor, flags), 10, &flags);
//                         break;
//                     case 'E':
//                         flags.is_up = true;
//                         s21_notat_float_to_str(str, get_arg_float(factor, flags), flags);
//                         break;
//                     case 'e':
//                         s21_notat_float_to_str(str, get_arg_float(factor, flags), flags);
//                         break;
//                     case 'f':
//                         s21_float_to_str(str, get_arg_float(factor, flags), flags);
//                         break;
//                     case 'G':
//                         flags.is_up = true;
//                         flags.is_g = true;
//                         s21_g_float_to_str(str, get_arg_float(factor, flags), &flags);
//                         break;
//                     case 'g':
//                         flags.is_g = true;
//                         s21_g_float_to_str(str, get_arg_float(factor, flags), &flags);
//                         break;
//                     case 'p':
//                         s21_ptr_to_str(str, va_arg(factor, void *), &flags);
//                         break;
//                     case 'n': {
//                         int *n = va_arg(factor, int *);
//                         if (n != S21_NULL) {
//                             *n = (int)(str - start_str + s21_strlen(str));
//                         }
//                         break;
//                     }
//                 }
//                 if (flags.is_up) {
//                     s21_upper_str(ptr);
//                 }
//                 state = 0;
//             } else if (state == 0) {
//                 s21_strncat(str, &(format[i]), 1);
//             } else {  // ошибка парсинга нужно вывести всё что было в строке
//                 s21_errors_sprintf(str, flags);
//                 s21_strncat(str, format + i, 1);
//                 state = 0;
//             }
//             i++;
//         }
//         va_end(factor);  // завершаем обработку параметров
//         len = str - start_str + s21_strlen(str);
//     }
//     return len;
// }


int s21_sprintf(char *str, const char *format, ...) {
    s21_size_t len = 0;
    if (str != S21_NULL && format != S21_NULL) {
        str[0] = '\0';
        char *start_str = str;
        va_list factor;            
        va_start(factor, format);  
        int i = 0;
        int state = 0;
        Flags flags;
        s21_zero_flags(&flags);

        while (format[i] != '\0') {
            if (format[i] == '%' && state == 0) {
                s21_zero_flags(&flags);
                state = 1;
            } else if (state == 1 && is_flag(format[i])) {
                s21_sprintf_flags(format[i], &flags);
            } else if (state == 1 && (is_digit(format[i]) || format[i] == '*')) {
                flags.width = (isdigit(format[i])) ? s21_sprintf_read_int(format, &i) : va_arg(factor, int);
                state = 2;
            } else if (state == 1 && format[i] == '.') {
                i++;
                flags.accuracy = (format[i] == '*') ? va_arg(factor, int) : s21_sprintf_read_int(format, &i);
                state = 3;
            } else if (state == 1 && is_length(format[i])) {
                flags.size = format[i];
                state = 4;
            } else if (state > 0 && is_specifier(format[i])) {
                // Call the helper function to handle the specifier
                process_specifier(&str, format[i], factor, flags, start_str);
                state = 0;
            } else if (state == 0) {
                strncat(str, &(format[i]), 1);
            } else {  
                // Error handling
                s21_errors_sprintf(str, flags);
                strncat(str, format + i, 1);
                state = 0;
            }
            i++;
        }
        va_end(factor);  
        len = str - start_str + s21_strlen(str);
    }
    return len;
}
void process_specifier(char **str, char specifier, va_list factor, Flags flags, char *start_str) {
    char *ptr = *str + s21_strlen(*str);

    switch (specifier) {
        case '%':
            s21_char_to_str(str, '%');
            break;
        case 'c':
            s21_char_to_str(str, va_arg(factor, int));
            break;
        case 's':
            if (flags.size == 'l') {
                wchar_t *wstr = va_arg(factor, wchar_t *);
                if (wstr != NULL) {
                    int buf_size = s21_wcslen(wstr);
                    char *buf = malloc(sizeof(char) * (buf_size + 1));
                    wcstombs(buf, wstr, buf_size + 1);
                    s21_str_to_str(*str, buf, &flags);
                    free(buf);
                } else {
                    s21_str_to_str(*str, NULL, &flags);
                }
            } else {
                s21_str_to_str(*str, va_arg(factor, char *), &flags);
            }
            break;
        case 'd':
        case 'i':
            s21_int_to_string(*str, get_arg_int(factor, flags), 10, &flags);
            break;
        case 'o':
            s21_int_to_string(*str, get_arg_uint(factor, flags), 8, &flags);
            break;
        case 'X':
            flags.is_up = true;
            s21_int_to_string(*str, get_arg_uint(factor, flags), 16, &flags);
            break;
        case 'x':
            s21_int_to_string(*str, get_arg_uint(factor, flags), 16, &flags);
            break;
        case 'u':
            flags.sign = false;
            s21_int_to_string(*str, get_arg_uint(factor, flags), 10, &flags);
            break;
        case 'E':
            flags.is_up = true;
            s21_notat_float_to_str(*str, get_arg_float(factor, flags), flags);
            break;
        case 'e':
            s21_notat_float_to_str(*str, get_arg_float(factor, flags), flags);
            break;
        case 'f':
            s21_float_to_str(*str, get_arg_float(factor, flags), flags);
            break;
        case 'G':
            flags.is_up = true;
            flags.is_g = true;
            s21_g_float_to_str(*str, get_arg_float(factor, flags), &flags);
            break;
        case 'g':
            flags.is_g = true;
            s21_g_float_to_str(*str, get_arg_float(factor, flags), &flags);
            break;
        case 'p':
            s21_ptr_to_str(*str, va_arg(factor, void *), &flags);
            break;
        case 'n': {
            int *n = va_arg(factor, int *);
            if (n != S21_NULL) {
                *n = (int)(*str - start_str + s21_strlen(*str));
            }
            break;
        }
    }

    if (flags.is_up) {
        s21_upper_str(ptr);
    }
}
