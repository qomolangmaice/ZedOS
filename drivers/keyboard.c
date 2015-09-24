/*
 *     File Name: keyboard.c
 *     Description: keyboard driver  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.08.20 Thursday 23:14:22
 */
#include "../libc/types.h"
#include "../libc/string.h"
#include "keyboard.h"
#include "keymap.h"
#include "../cpu/isr.h" 
#include "../cpu/irq.h" 
//#include "screen.h" 
#include "tty.h" 

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

uint8 get_byte_from_kbuf(); 
static void kb_wait();  		/* Wait for the buffer in 8042 is empty */
static void kb_ack(); 
static void set_leds();  

/*======================================================================*
                            keyboard_handler
 *======================================================================*/
void keyboard_handler(registers_t regs)
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
}

/*======================================================================*
                           init_keyboard
*======================================================================*/
void init_keyboard()
{
	/* Initialize the kbinput structure members */
	kbinput.count = 0;
	kbinput.p_head = kbinput.p_tail = kbinput.buf;

	/* Initialize Shift(L, R) key, Alt(L, R) key, Ctrl(L, R) key */
	shift_l = shift_r = 0; 
	alt_l = alt_r = 0; 
	ctrl_l = ctrl_r = 0; 

	/* Initialize the leds state of Caps lock, Nums lock, Scroll lock key */
	caps_lock = 0; 
	num_lock = 1; 
	scroll_lock = 0; 

	/* Initialize the keyboard leds state */
	set_leds(); 

	/* IRQ1 is the number of keyboard interrupt */
	register_interrupt_handler(IRQ1, &keyboard_handler);	/* Setup keyboard handler */
}

/*======================================================================*
                           keyboard_read
*======================================================================*/

/* Analysis ScanCode */
void keyboard_read(TTY *p_tty)
{
	uint8 scan_code;
 	uint8 make;	/* TRUE : make , FALSE: break */

	uint32 key = 0; 	/* An integer variable is represent for a key */
	uint32 *keyrow; 	/* Point to the line of keymap[] */ 

	
	if(kbinput.count > 0)
	{
		code_with_E0 = 0; 

		scan_code = get_byte_from_kbuf(); 
		
		/* analysis scancode */
		if (scan_code == 0xE1) 
		{
			uint32 i; 
			uint8 pausebrk_scode[] = {0xE1, 0x1D, 0x45, 
				 	 	 	 	 	  0xE1, 0x9D, 0xC5}; 
			uint32 is_pausebreak = 1; 
			for(i=1; i<6; i++)
			{
				if(get_byte_from_kbuf() != pausebrk_scode[i])
				{
					is_pausebreak = 0; 
					break; 
				}
			}
			if(is_pausebreak)
			{
				key = PAUSEBREAK; 
			}
		}
		else if (scan_code == 0xE0) 
		{
			scan_code = get_byte_from_kbuf(); 

			/* Press 'PrintScreen key */
			if(scan_code == 0x2A)
			{
				if(get_byte_from_kbuf() == 0xE0)
				{
					if(get_byte_from_kbuf() == 0x37)
					{
						key = PRINTSCREEN; 
						make = 1; 
					}
				}
			}

			/* Release 'PrintScreen' key */
			if(scan_code == 0xB7)
			{
				if(get_byte_from_kbuf() == 0xE0)
				{
					if(get_byte_from_kbuf() == 0xAA)
					{
						key = PRINTSCREEN; 
						make = 0; 
					}
				}
			}

			/*  */
			if(key == 0)
			{
				code_with_E0 = 1; 
			}
		}
		if ((key != PAUSEBREAK) && (key != PRINTSCREEN))  
		{	
			/* handle charactor that can be printed */
			/* Judge the scancode is Make Code or Break Code */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);

			/* Firstly, Locate the rows of keymap array */
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS]; 

			column = 0; 

			int caps = shift_l || shift_r; 

			/* Set led by Caps lock key */
			if(caps_lock)
			{
				if((keyrow[0] >= 'a') && (keyrow[0] <= 'z')) 
				{
					caps = !caps; 
				}
			}

			if(caps)
			{
				column = 1; 
			}
			if(code_with_E0)
			{
				column = 2; 
				//code_with_E0 = 0; 
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
				case CAPS_LOCK: 
					if(make) 
					{
						caps_lock = !caps_lock; 
						set_leds(); 
					}
					break; 
				case NUM_LOCK: 
					if(make) 
					{
						num_lock = !num_lock; 
						set_leds(); 
					}
					break; 
				case SCROLL_LOCK: 
					if(make)
					{
						scroll_lock = !scroll_lock; 
						set_leds(); 
					}
					break; 
				default: 
					break; 
			}

			/* If the key exsits(is not equal to 0), then prints it */
			if(make) 	/* Ignore Break Code */ 
			{
				int pad = 0; 
				if((key >= PAD_SLASH) && (key <= PAD_9)) 
				{
					pad = 1; 
					switch(key) 
					{
						case PAD_SLASH: 
							key = '/'; 
							break; 
						case PAD_STAR: 
							key = '*'; 
							break; 
						case PAD_MINUS: 
							key = '-'; 
							break; 
						case PAD_PLUS: 
							key = '+'; 
							break; 
						case PAD_ENTER: 
							key = ENTER; 
							break; 
						default: 
							if(num_lock && 
								(key >= PAD_0) && 
								(key <= PAD_9)) 
							{
								key = key - PAD_0 + '0'; 
							}
							else if(num_lock && 
									(key == PAD_DOT)) 
							{
								key = '.'; 
							}
							else 
							{
								switch(key)
								{
									case PAD_HOME: 
										key = HOME; 
										break; 
									case PAD_END: 
										break; 
									case PAD_PAGEUP: 
										key = PAGEUP; 
										break; 
	 	 	 	 	 	 	 	 	case PAD_PAGEDOWN: 
										key = PAGEDOWN; 
										break; 
									case PAD_INS: 
										key = INSERT; 
										break; 
									case PAD_UP: 
										key = UP; 
										break; 
									case PAD_DOWN: 
										key = DOWN; 
										break; 
									case PAD_LEFT: 
										key = LEFT; 
										break; 
									case PAD_RIGHT: 
										key = RIGHT; 
										break; 
									case PAD_DOT: 
										key = DELETE; 
										break; 
									default: 
										break; 
								}
							}
							break; 
					}
				}
				key |= shift_l ? FLAG_SHIFT_L : 0; 
				key |= shift_r ? FLAG_SHIFT_R : 0; 
				key |= ctrl_l  ? FLAG_CTRL_L  : 0; 
				key |= ctrl_r  ? FLAG_CTRL_L  : 0; 
				key |= alt_l   ? FLAG_ALT_L   : 0; 
				key |= alt_r   ? FLAG_ALT_R   : 0; 

				in_process(p_tty, key); 	/* In tty.c */ 
			}
		}
	}
}

uint8 get_byte_from_kbuf() 	/* Read the next byte from keyboard buffer */	
{
	uint8 scan_code; 

	while(kbinput.count <= 0) {} 	/* Wait for the next byte  */

 	scan_code = *(kbinput.p_tail);

	kbinput.p_tail++;
	if (kbinput.p_tail == kbinput.buf + KB_IN_BYTES) 
	{
		kbinput.p_tail = kbinput.buf;
	}
	kbinput.count--;
	
	return scan_code; 
}

static void kb_wait() 	/* Wait for the buffer in 8042 is empty */
{
	uint8 kb_stat; 

	do{
		kb_stat = inportb(KB_CMD); 
 	} while (kb_stat & 0x02); 	
}

static void kb_ack() 
{
	uint8 kb_read; 

 	do{
		kb_read = inportb(KB_DATA); 
 	} while (kb_read != KB_ACK); 	
}

static void set_leds()
{
	uint8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock; 

	kb_wait(); 
	outportb(KB_DATA, LED_CODE); 
	kb_ack(); 
	kb_wait(); 
	outportb(KB_DATA, leds); 
	kb_ack(); 
}
