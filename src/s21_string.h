#ifndef s21_string
#define s21_string

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>


char *s21_strerror(int errnum);
size_t s21_strlen(const char *str);
int s21_sscanf(const char *str, const char *format, ...);
int s21_sprintf(char *str, const char *format, ...);

#endif