#ifndef s21_string
#define s21_string

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include <string.h>

#define is_flag(c) \
  ((c) == '-' || (c) == '+' || (c) == ' ' || (c) == '#' || (c) == '0')
#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define is_length(c) ((c) == 'h' || (c) == 'l' || (c) == 'L')
#define is_specifier(c)                                                  \
  ((c) == 'c' || (c) == 'd' || (c) == 'i' || (c) == 'e' || (c) == 'E' || \
   (c) == 'f' || (c) == 'g' || (c) == 'G' || (c) == 'o' || (c) == 's' || \
   (c) == 'u' || (c) == 'x' || (c) == 'X' || (c) == 'p' || (c) == 'n' || \
   (c) == '%')
#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')

#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;

int s21_sscanf(const char *str, const char *format, ...);
int s21_sprintf(char *str, const char *format, ...);
void *s21_memchr(const void *str, int c, size_t n);
int s21_memcmp(const void *str1, const void *str2, size_t n);
void *s21_memcpy(void *dest, const void *src, size_t n);
char *s21_strerror(int errnum);
size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

#endif