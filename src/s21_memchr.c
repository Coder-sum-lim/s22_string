#define NULL ((void *)0)
typedef unsigned long size_t;

void *s21_memchr(const void *str, int c, size_t n) {
    const unsigned char *string = str;
    unsigned char h = c;
    int flag = 0;

    for (size_t i = 0; i < n; i++) {
        if (string[i] == h){
            flag = 1;
            string = string + i;
            break;
        }
    }
    return flag == 1 ? (void *)(string) : NULL;
}