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

int32 disp_pos = 0; 
static void set_cursor(uint32 position); 

int32 is_current_console(CONSOLE *p_con)
{
	return (p_con == &console_table[nr_current_console]); 
}

void out_char(CONSOLE *p_con, char ch)
{
	uint8 *p_vmem = (uint8 *)(V_MEM_BASE + disp_pos); 

	*p_vmem++ = ch; 
	*p_vmem++ = DEFAULT_CHAR_COLOR; 
	disp_pos += 2; 

	set_cursor(disp_pos/2); 
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
