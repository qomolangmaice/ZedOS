/*
 *     File Name: keyboard.c
 *     Description: keyboard driver  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月20日 星期四 23时14分22秒
 */
#include "../libc/types.h"
#include "../libc/string.h"
#include "keyboard.h"
#include "keymap.h"
#include "../cpu/isr.h" 
#include "../cpu/irq.h" 
#include "screen.h" 

#define KB_INPUT_PORT  0x60  

static KB_INPUT kbinput; 
/*======================================================================*
                            keyboard_handler
 *======================================================================*/
void keyboard_handler(registers_t *regs)
{
	uint8 scan_code = inportb(KB_INPUT_PORT);

	/* If the kbinput count is not full, then read byte from keyboard */
	if (kbinput.count < KB_IN_BYTES) 
	{
		*(kbinput.p_head) = scan_code;
		kbinput.p_head++;

		/* If the kbinput buffer is full, then ignore input bytes */
		if (kbinput.p_head == kbinput.buf + KB_IN_BYTES) 
		{
			kbinput.p_head = kbinput.buf;
		}
		kbinput.count++;
	}

	keyboard_read();
}

/*======================================================================*
                           init_keyboard
*======================================================================*/
void init_keyboard()
{
	/* Initialize the kbinput structure members */
	kbinput.count = 0;
	kbinput.p_head = kbinput.p_tail = kbinput.buf;

	/* IRQ1 is the number of keyboard handler,   */
	register_interrupt_handler(IRQ1, keyboard_handler);	/* Setup keyboard handler */
}

/*======================================================================*
                           keyboard_read
*======================================================================*/
void keyboard_read()
{
	uint8	scan_code;
	char	output[2];
 	uint8	make;	/* TRUE : make , FALSE: break */
	
	/* Clean output array */
	memory_set(output, 0, 2);
	if(kbinput.count > 0)
	{
		scan_code = *(kbinput.p_tail);

		kbinput.p_tail++;
		if (kbinput.p_tail == kbinput.buf + KB_IN_BYTES) 
		{
			kbinput.p_tail = kbinput.buf;
		}
		kbinput.count--;

		/* analysis scancode */
		if (scan_code == 0xE1) 
		{
			/* Not to do anything right now */
		}
		else if (scan_code == 0xE0) 
		{
			/* Not to do anything right now */
		}
		else 
		{	
			/* handle charactor that can be printed */
			/* If ScanCode is Make Code, then prints it, 
			 * and if Scancode is Break Code, not to do anything now */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
			if(make)
			{
				output[0] = keymap[(scan_code & 0x7F) * MAP_COLS];
				print(output);
			}
		}
	}
}
