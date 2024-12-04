#ifndef s21_string
#define s21_string

#include <stdio.h>
#include <string.h>

char *s21_strerror(int errnum);
size_t s21_strlen(const char *str);
void *memchr(const void *str, int c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif