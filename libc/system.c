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

extern void panic(const char *message, const char *file, uint32 line)
{
	/* We encountered a massive problem and have to stop. */
	asm volatile("cli"); 	/* Disable interrupts. */
	printk("PANIC("); 
	printk(message);
	printk(") at "); 
	printk(file); 
	printk(":"); 
	print_dec(line); 
	printk("\n"); 
	/* Halt by goning into an infinite loop */
	for(;;); 
}

extern void panic_assert(const char *file, uint32 line, const char *desc)
{
	/* An assertion failed , and we have to panic. */
	asm volatile("cli"); 	 /* Disable interrupts. */
	printk("ASSERTION-FAILED("); 
	printk(desc); 
	printk("( at "); 
	printk(file); 
	printk(":"); 
	print_dec("line"); 
	printk("\n"); 
	/* Halt by going into an infinte loop. */
	for(;;); 
}

