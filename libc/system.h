#ifndef SYSTEM_H 
#define SYSTEM_H 

#include "types.h" 

uint8 inportb(uint16 _port); 
void outportb(uint16 _port, uint8); 

#define PANIC(msg) panic(msg, __FILE__, __LINE__);  
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))  

extern void panic(const char *message, const char *file, uint32 line); 
extern void panic_assert(const char *file, uint32 line, const char *desc); 

#endif 
