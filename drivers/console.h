/*
 *     File Name: console.h
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:25 Tue 22:03:34 
 */
#ifndef CONSOLE_H 
#define CONSOLE_H 
#define NR_CONSOLES  3 	 	/* consoles */
#define DEFAULT_CHAR_COLOR 	0x02 	/* 0000 0111 Black background and green word */
#define SCR_UP 1 	/* scroll forward */
#define SCR_DN -1 	/* scroll backward */

#define SCREEN_SIZE 	(80*25) 
#define SCREEN_WIDTH 	80 

#include "../libc/types.h" 
#include "tty.h" 

typedef struct s_console
{
	uint32 current_start_addr; 	 	/* Current display location */ 
	uint32 original_addr; 	 	 	/* The current video address of console */
	uint32 v_mem_limit; 	 	  	/* The video memory size of the current console */	
	uint32 cursor;  	 	 	 	/* Current cursor location */
}CONSOLE; 

CONSOLE console_table[NR_CONSOLES];  

int nr_current_console; 

int32 is_current_console(CONSOLE *p_con);  

void out_char(CONSOLE *p_con, char ch);  

void select_console(int nr_console); 	

void scroll_screen(CONSOLE* p_con, int direction);  

static void flush(CONSOLE *p_tty);  

static void set_cursor(uint32 position); 

static void set_video_start_addr(uint32 addr); 

#endif 
