/*
 *     File Name: console.c
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:26 Wed 19:47:45 
 */
#include "console.h" 
#include "../libc/types.h" 
#include "../libc/system.h" 
#include "keyboard.h" 
#include "../libc/types.h" 
#include "tty.h" 

//void init_screen(TTY *p_tty);  

/* The cursor default position in tty0 */
int32 disp_pos = 1440; 

void init_screen(TTY *p_tty) 
{
	int nr_tty = p_tty - tty_table; 
	p_tty->p_console = console_table + nr_tty; 

	int v_mem_size = V_MEM_SIZE >> 1; 

	int con_v_mem_size = v_mem_size / NR_CONSOLES; 
	p_tty->p_console->original_addr = nr_tty * con_v_mem_size; 
	p_tty->p_console->v_mem_limit = con_v_mem_size; 
	p_tty->p_console->current_start_addr = p_tty->p_console->original_addr; 
	p_tty->p_console->cursor = p_tty->p_console->original_addr; 

	if(nr_tty == 0) 
	{
		/* The cursor location of the first console */
		p_tty->p_console->cursor = disp_pos / 2; 
		disp_pos = 0; 
	}
	else
	{
		out_char(p_tty->p_console, nr_tty + '0'); 
		out_char(p_tty->p_console, '#'); 
		out_char(p_tty->p_console, ' '); 
	}
	set_cursor(p_tty->p_console->cursor); 
}

/* return the pointer of current console */
int32 is_current_console(CONSOLE *p_con)
{
	return (p_con == &console_table[nr_current_console]); 
}

void out_char(CONSOLE *p_con, char ch)
{
	uint8 *p_vmem = (uint8 *)(V_MEM_BASE + p_con->cursor * 2); 

	switch(ch)
	{
		case '\n': 
			/* Enter: Move the cursor to the next line */
			if(p_con->cursor < p_con->original_addr + 
			   p_con->v_mem_limit - SCREEN_WIDTH) 
			{
				p_con->cursor = p_con->original_addr + SCREEN_WIDTH *
					((p_con->cursor - p_con->original_addr) / 
					 SCREEN_WIDTH + 1); 
			}
			break; 
		case '\b': 
			/* Backspace: Delete the previous character */
			if(p_con->cursor > p_con->original_addr) 
			{
				p_con->cursor--; 
				*(p_vmem - 2) = ' '; 
				*(p_vmem - 1) = DEFAULT_CHAR_COLOR; 
			}
			break; 
		default: 
			/* Output the character to the console screen */
			if(p_con->cursor < 
			   p_con->original_addr + p_con->v_mem_limit - 1) 
			{
				*p_vmem++ = ch; 
			 	*p_vmem++ = DEFAULT_CHAR_COLOR; 
				p_con->cursor++; 
			}
			break; 
	}
	
	/* If the cursor location is at the end of current screen 
	 * then scroll the screen to the next line
	 */
	while (p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE) 
	{
		scroll_screen(p_con, SCR_DN); 
	}

	flush(p_con); 
}

/*  Update the cursor in the screen */
static void flush(CONSOLE *p_con) 
{
	set_cursor(p_con->cursor); 
	set_video_start_addr(p_con->current_start_addr); 
}

/* Setup the cursor location */
static void set_cursor(uint32 position)
{
	asm volatile ("cli"); 	/* Close all interrupts */ 
	outportb(CRTC_ADDR_REG, CURSOR_H); 
	outportb(CRTC_DATA_REG, (position >> 8) & 0xFF); 
	outportb(CRTC_ADDR_REG, CURSOR_L); 
	outportb(CRTC_DATA_REG, position & 0xFF); 
	asm volatile ("sti"); 	/* Start the interrupts  */ 
}

/* Setup the cursor to the start of video memory address */
static void set_video_start_addr(uint32 addr)
{
	asm volatile ("cli"); 
	outportb(CRTC_ADDR_REG, START_ADDR_H); 
	outportb(CRTC_DATA_REG, (addr >> 8) & 0xFF); 
	outportb(CRTC_ADDR_REG, START_ADDR_L); 
	outportb(CRTC_DATA_REG, addr & 0xFF); 
	asm volatile ("sti"); 
}

void select_console(int nr_console) 	/* 0 - (NR_CONSOLES - 1) */	
{
	if((nr_console < 0) || (nr_console >= NR_CONSOLES)) 
		return;  

	nr_current_console = nr_console; 

	set_cursor(console_table[nr_console].cursor); 
	set_video_start_addr(console_table[nr_console].current_start_addr); 
}

void scroll_screen(CONSOLE* p_con, int direction)
{
	/* Scroll up a line */
	if(direction == SCR_UP) 
	{
		if(p_con->current_start_addr > p_con->original_addr)
		{
			p_con->current_start_addr -= SCREEN_WIDTH; 
		}
	}

	/* Scroll down a line */
	else if (direction == SCR_DN)
	{
		if(p_con->current_start_addr + SCREEN_SIZE <
		   p_con->original_addr + p_con->v_mem_limit) 
		{
			p_con->current_start_addr += SCREEN_WIDTH; 
		}
	}
	else {}

	flush(p_con); 
	//set_video_start_addr(p_con->current_start_addr); 
	//set_cursor(p_con->cursor); 
}
