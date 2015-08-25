/*
 *     File Name: tty.h
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:25 Tue 21:55:30 
 */
#ifndef TTY_H 
#define TTY_H 
#include "../libc/types.h" 
#include "console.h" 

#define TTY_IN_BYTES 256 	/* tty input queue size */
#define NR_CONSOLES  3 	 	/* consoles */

struct s_console; 

/* TTY */
typedef struct s_tty
{
	uint32 in_buf[TTY_IN_BYTES]; 	/* TTY input buffer */
	uint32 *p_inbuf_head; 	 	 	/* Point to the next empty */
	uint32 *p_inbuf_tail;  	 	 	/* Point to the key value  */
	int inbuf_count; 	 	 	 	

	struct s_console *p_console; 
}TTY; 

TTY tty_table[NR_CONSOLES]; 

static void init_tty(TTY *p_tty); 
static void tty_do_read(TTY *p_tty); 
static void tty_do_write(TTY *p_tty); 

#endif 