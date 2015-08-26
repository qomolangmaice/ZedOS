/*
 *     File Name: tty.c
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:25 Tue 23:12:29 
 */

#include "tty.h" 
#include "console.h" 
#include "keyboard.h" 
#include "screen.h" 

#define TTY_FIRST 	(tty_table) 
#define TTY_END   	(tty_table + NR_CONSOLES) 
extern int nr_current_console; 

void task_tty() 
{
	TTY *p_tty; 

	init_keyboard(); 

	for(p_tty=TTY_FIRST; p_tty<TTY_END; p_tty++)
	{
		init_tty(p_tty); 
	}
	nr_current_console = 0; 
	while(1)
	{
		for(p_tty=TTY_FIRST; p_tty<TTY_END; p_tty++)
		{
			tty_do_read(p_tty); 
			tty_do_write(p_tty); 
		}
	}
}

static void init_tty(TTY *p_tty)
{
 	p_tty->inbuf_count = 0; 
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf; 

	int nr_tty = p_tty - tty_table; 
	p_tty->p_console = console_table + nr_tty; 
}

void in_process(TTY *p_tty, uint32 key)
{
	char output[2] = {'\0', '\0'}; 

	if(!(key & FLAG_EXT))
	{
		if(p_tty->inbuf_count < TTY_IN_BYTES) 
		{
			*(p_tty->p_inbuf_head) = key; 
			p_tty->p_inbuf_head++; 
			if(p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES)
			{
				p_tty->p_inbuf_head = p_tty->in_buf; 
			}
			p_tty->inbuf_count++; 
		}
	}
	else
	{
		int raw_code = key & MASK_RAW; 
		switch(raw_code)
		{
			case UP:
				if((key & FLAG_SHIFT_L) || (key &FLAG_SHIFT_R))
				{
					asm volatile ("cli"); 
					outportb(CRTC_ADDR_REG, START_ADDR_H); 
					outportb(CRTC_DATA_REG, ((80*15) >> 8) & 0xFF); 
					outportb(CRTC_ADDR_REG, START_ADDR_L); 
					outportb(CRTC_DATA_REG, (80*15) & 0xFF); 
					asm volatile ("sti"); 
				}
				break; 
 	 	 	case DOWN:
				if((key & FLAG_SHIFT_L) || (key &FLAG_SHIFT_R))
				{
				
				}
				break; 
			default: 
				break; 
		}
	}
}

static void tty_do_read(TTY *p_tty)
{
	if(is_current_console(p_tty->p_console))
	{
		keyboard_read(p_tty); 
	}
}
static void tty_do_write(TTY *p_tty)
{
	if(p_tty->inbuf_count)
	{
		char ch = *(p_tty->p_inbuf_tail); 
		p_tty->p_inbuf_tail++; 
		if(p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES)
		{
			p_tty->p_inbuf_tail = p_tty->in_buf; 
		}
		p_tty->inbuf_count++; 
		out_char(p_tty->p_console, ch); 
	}
}
