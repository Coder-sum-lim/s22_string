#define NULL ((void *)0)
typedef unsigned long size_t;

int memcmp(const void *str1, const void *str2, size_t n){
    const char *example1 = str1;
    const char *example2 = str2;
    int flag = 0;
    for (size_t i = 0; i < n; i++){
        if (*(example1 +i) != *(example2 +i)){
            flag = ((*(example1 + i) < *(example2 + i))) ? -1 : 1;
            break;
        }
    }
    return flag;
}