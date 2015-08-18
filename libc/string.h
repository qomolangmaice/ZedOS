#ifndef STRING_H 
#define STRING_H 

#include "types.h" 

void memory_set(void *dest, uint8 val, uint32 len);  
uint16 str_len(string ch);  

void int_to_ascii(int n, char str[]);  

uint8 str_equal(string ch1, string ch2);   

void backspace(char s[]); 
void append(char s[], char n); 
int strcmp(char s1[], char s2[]);  

#endif 
