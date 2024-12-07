#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
    char *iserr = S21_NULL;
    int flag=0;
    if (str1 == S21_NULL || str2 == S21_NULL) {
        iserr = S21_NULL;
    } else {
        while (*str1 != '\0') {
            for (int i = 0; str2[i] != '\0'; i++) {
                if (*str1 == str2[i]) {
                    iserr = (char *)str1;
                    flag=1;
                }
            }
            str1++;
            if (flag) break;
        }
    }
    return iserr;
}