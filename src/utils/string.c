#include "string.h"
#include <stdint.h>
char* strncpy(char* dest, const char* src, size_t n) {
    size_t i = 0;

    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    while (i < n) {
        dest[i++] = '\0';
    }

    return dest;
}

char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++));
    return ret;
}


int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}
void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }

    return dest;
}
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < num; ++i)
        p[i] = (unsigned char)value;
    return ptr;
}
int toupper(int c) {
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');
    return c;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i])
            return p1[i] - p2[i];
    }
    return 0;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}
char* strrchr(const char* str, int ch) {
    const char* last = NULL;
    while (*str) {
        if (*str == (char)ch)
            last = str;
        ++str;
    }
    return (char*)last;
}
