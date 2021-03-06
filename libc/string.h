#ifndef STRING_H 
#define STRING_H 

#include "types.h" 

void memory_set(void *dest, uint8 val, uint32 len); 

void memcpy(uint8 *dest, const uint8 *src, uint32 len);  

char *strcpy(char *dest, const char *src);  

uint16 str_len(string ch);  

inline int strlen(const char *src);  

void int_to_ascii(int n, char str[]);  

uint8 str_equal(string ch1, string ch2);   

void backspace(char s[]); 
void append(char s[], char n); 
int strcmp(char s1[], char s2[]);  

#endif 
