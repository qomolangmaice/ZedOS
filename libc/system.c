#include "system.h" 
#include "types.h" 

/* In intel syntax */  
/* global inportb 
 *
 * inportb: 
 * 	 	mov edx, [esp + 4] 
 * 	 	in al, dx 
 * 	 	ret 
 */

/* In AT&T syntax */
uint8 inportb(uint16 _port) 
{
 	uint8 ret_val; 
	__asm__ __volatile__ ("inb %1, %0" : "=a" (ret_val) : "dN" (_port)); 
	return ret_val;  
}

/* In intel syntax */  
/* global outportb
 *
 * outportb: 
 *  	 mov edx, [esp + 4] 
 *  	 mov al, [esp + 4 + 4] 
 *  	 out dx, al 
 *  	 ret 
 */

/* In AT&T syntax */
void outportb(uint16 _port, uint8 _data)
{
 	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));  
}


