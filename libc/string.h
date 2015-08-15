#ifndef STRING_H 
#define STRING_H 

#include "types.h" 

void memory_set(void *dest, uint8 val, uint32 len);  
uint16 str_len(string ch);  

// Added in episode 3
// This function compares two strings 
// and returns true (1) if they are equal 
// or false (0) if they are not equal 
//
uint8 str_equal(string ch1, string ch2);   

#endif 
