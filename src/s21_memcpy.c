#define NULL ((void *)0)
typedef unsigned long size_t;

void *memcpy(void *dest, const void *src, size_t n){
    for(size_t i = 0; i < n; i++){
        src
    }
}


#include <stdio.h>
#include <string.h>
int main()
{
 //the source buffer
 char src[6] = "Privet";
 //dst buffer
 char dst[15] = "Hello , world";
 //copy the source buffer int dst
 memcpy(dst,src,sizeof(src));
 //print the dst buffer
 printf("dst = %s\n", dst);
 return 1;
}