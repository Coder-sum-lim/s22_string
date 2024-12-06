#define NULL ((void *)0)
typedef unsigned long size_t;

void *memcpy(void *dest, const void *src, size_t n){
    char *edest = dest;
    const char *esrc = src;
    for(size_t i = 0; i < n; i++) *edest++ = *esrc++;

    return dest;
}
