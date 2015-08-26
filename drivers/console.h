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
#define DEFAULT_CHAR_COLOR 	0x07 	/* 0000 0111 Black background and white word */
#include "../libc/types.h" 

typedef struct s_console
{
	uint32 current_start_addr; 	 
	uint32 original_addr; 	 	 	/* Current console video address */
	uint32 v_mem_limit; 	 	 	
	uint32 cursor;  	 	 	 	/* Current cursor location */
}CONSOLE; 

CONSOLE console_table[NR_CONSOLES];  

int nr_current_console; 
int32 is_current_console(CONSOLE *p_con);  
void out_char(CONSOLE *p_con, char ch);  

#endif 
