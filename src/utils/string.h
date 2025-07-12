#ifndef __STRING_H
#define __STRING_H

#include <stddef.h>

char* strncpy(char* dest, const char* src, size_t n);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t num);
int toupper(int c);
int memcmp(const void* s1, const void* s2, size_t n);
size_t strlen(const char* str);
char* strrchr(const char* str, int ch);
#endif
