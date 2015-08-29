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

/* Current cursor position */
int32 disp_pos = 1440; 

static void set_cursor(uint32 position); 
static void set_video_start_addr(uint32 addr); 
void init_screen(TTY *p_tty);  

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

int32 is_current_console(CONSOLE *p_con)
{
	return (p_con == &console_table[nr_current_console]); 
}

void out_char(CONSOLE *p_con, char ch)
{
	uint8 *p_vmem = (uint8 *)(V_MEM_BASE + p_con->cursor * 2); 

	*p_vmem++ = ch; 
	*p_vmem++ = DEFAULT_CHAR_COLOR; 
	p_con->cursor++; 

	set_cursor(p_con->cursor); 
}

static void set_cursor(uint32 position)
{
	asm volatile ("cli"); 
	outportb(CRTC_ADDR_REG, CURSOR_H); 
	outportb(CRTC_DATA_REG, (position >> 8) & 0xFF); 
	outportb(CRTC_ADDR_REG, CURSOR_L); 
	outportb(CRTC_DATA_REG, position & 0xFF); 
	asm volatile ("sti"); 
}

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
	if(direction == SCR_UP) 
	{
		if(p_con->current_start_addr > p_con->original_addr)
		{
			p_con->current_start_addr -= SCREEN_WIDTH; 
		}
	}
	else if (direction == SCR_DN)
	{
		if(p_con->current_start_addr + SCREEN_SIZE <
		   p_con->original_addr + p_con->v_mem_limit) 
		{
			p_con->current_start_addr += SCREEN_WIDTH; 
		}
	}
	else {}

	set_video_start_addr(p_con->current_start_addr); 
	set_cursor(p_con->cursor); 
}
