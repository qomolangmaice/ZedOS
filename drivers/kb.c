/*
 *     File Name: kb.c
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月18日 星期二 21时03分09秒
 */
#include "kb.h" 
#include "../drivers/screen.h" 
#include "../cpu/irq.h" 

unsigned char kbdus[128] =
{
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', 
	'9', '0', '-', '=', '\b', /* Backspace */ 
	'\t', 	 	/* Tab */
	'q', 'w', 'e', 'r', /* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 	/* Enter key */
	0,  	    /* 29 - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 	/* 39 */
	'\'', '`', 0,  	 	/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 	 	 	/* 49 */
	'm', ',', '.', '/', 0, 	 	 	 	 	 	 	/* Right shift */
	'*', 
	  0, 	/* Alt */ 
	' ',  	/* Space bar */ 
	  0,  	/* Caps lock */
	  0,    /* 59 - F1 key ...> */ 
	  0, 0, 0, 0, 0, 0, 0, 0, 
	  0,    /* < ... F10  */
	  0, 	/* Scroll Lock */ 
	  0,  	/* Home key */
	  0,  	/* Up Arrow */
	  0,  	/* Page Up  */
	 '-',  
	  0,  	/* Left Arrow */
	  0,  	
	  0,  	/* Right Arrow */
	 '+',  
	  0,  	/* 79 -End key */
	  0,  	/* Down Arrow */
	  0,  	/* Page Down */
	  0,  	/* Inert Key */
	  0,  	/* Delete Key */
	  0, 0, 0   
	  0,  	/* F11 Key */
	  0,  	/* F12 Key */
	  0,  	/* All other keys are undefined */
}; 

static void keyboard_handler(registers_t *regs)
{
 	unsigned char scancode; 

	/*  Read from the keyboard's data buffer */
	scancode = inportb(0x06); 

	/* If the top bit of the byte we read from the keyboard is 
	 * set, that means that a key has just been released */
	if (scancode & 0x80)
	{
		/* You can use this one to see if the user released the 
		 * shift, altm orm control keys ... */

	}
	else 
	{
		/* Here, a key ws just pressed. Please note that if you 
		 * hold a key down, you will get repeated key press interrupt 
		 */
		print(kbdus[scancode]); 
	}
}

void keyboard_install()
{
	regigster_interrupt_handler(IRQ1, keyboard_handler); 
}
