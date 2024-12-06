#ifndef s21_string
#define s21_string

#include <string.h>
#include <stdio.h>

#define NULL ((void *)0)
typedef unsigned long size_t;

void *s21_memchr(const void *str, int c, size_t n);
int s21_memcmp(const void *str1, const void *str2, size_t n);
void *s21_memcpy(void *dest, const void *src, size_t n);
char *s21_strerror(int errnum);
size_t s21_strlen(const char *str);

#endif