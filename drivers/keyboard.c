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
static int code_with_E0 = 0; 	
static int shift_l; 	 	 	/* left shift state  */ 
static int shift_r; 	 	 	/* right shift state */ 
static int alt_l;  	 	 	 	/* left alt state */ 
static int alt_r; 	 	 	    /* right alt state */ 
static int ctrl_l; 	 	 	 	/* left ctrl state */ 
static int ctrl_r; 	 	 	 	/* right ctrl state */ 
static int caps_lock; 	 	 	/* Caps Lock */ 
static int num_lock; 	 	 	/* Num Lock */ 
static int scroll_lock; 	 	/* Scroll Lock */ 
static int column; 	 	 	 	
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

/* Analysis ScanCode */
void keyboard_read()
{
	uint8 scan_code;
	char  output[2];
 	uint8 make;	/* TRUE : make , FALSE: break */

	uint32 key = 0; 	/* An integer variable is represent for a key */
	uint32 *keyrow; 

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

			/* Firstly, Locate in the rows of keymap array */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS]; 

			column = 0; 
			if(shift_l || shift_r)
			{
				column = 1; 
			}
			if(code_with_E0)
			{
				column = 2; 
				code_with_E0 = 0; 
			}

			key = keyrow[column]; 

			switch(key) 
			{
				case SHIFT_L:
					shift_l = make; 
					key = 0; 
					break; 
				case SHIFT_R:  
					shift_r = make; 
					key = 0; 
					break; 
 	 	 	 	case CTRL_L:
					ctrl_l = make; 
					key = 0; 
					break; 
				case CTRL_R:  
					ctrl_r = make; 
					key = 0; 
					break; 
 	 	 	 	case ALT_L:
					alt_l = make; 
					key = 0; 
					break; 
				case ALT_R:  
					alt_r = make; 
					key = 0; 
					break; 
				default: 
					if(!make) 
					{
						key = 0;  	/* If scancode is BreakCode, ignore it  */
					}
					break; 
			}

			/* If key exsits(is not equal to 0), then prints it */
			if(key) 
			{
				output[0] = key; 
				print(output); 
			}
		}
	}
}
