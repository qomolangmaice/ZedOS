#include "system.h" 
#include "types.h" 

uint8 inportb(uint16 _port) 
{
 	uint8 ret_val; 
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret_val) : "dN" (_port)); 
	return ret_val;  
}

void outportb(uint16 _port, uint8 _data)
{
 	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));  
}


