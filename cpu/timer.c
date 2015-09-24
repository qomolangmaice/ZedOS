/*
 *     File Name: timer.c
 *     Description: PIT related  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月16日 星期日 23时47分17秒
 */
#include "timer.h" 
#include "isr.h" 
#include "irq.h" 
#include "../libc/system.h" 
#include "../libc/string.h"
#include "../libc/types.h" 
#include "../libc/printf.h" 

void timer_callback(registers_t regs)
{
	static uint32 tick = 0; 
 	printf("Tick: %d\n", tick++); 
}

void init_timer(uint32 frequency)
{
	/* interrupt handler function about registe timer */ 
	register_interrupt_handler(IRQ0, &timer_callback); 

 	/* Intel 8253/8254 PIT chip 
	 * I/O port address: 0x40h ~ 0x43h 
	 * Input frequency: 1193180, frequency is the counts per second */
	uint32 divisor = 1193180 / frequency; 

	/* D7 D6 D5 D4 D3 D2 D1 D0  
	 * 0  0  1  1  0  1  1  0   
	 * Above binary code is 0x36H 
	 * setup 8253/8254 chip work in Mode 3
	 */ 
	outportb(0x43, 0x36); 

	/* divide low bytes and high bytes */
	uint8 low = (uint8)(divisor & 0xFF); 
	uint8 high = (uint8)((divisor >> 8) & 0xFF); 

	/*
	 * write into low bytes 
	 * write into high bytes 
	 */
	outportb(0x40, low); 
	outportb(0x40, high); 
}
