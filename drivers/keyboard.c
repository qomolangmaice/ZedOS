/*
 *     File Name: keyboard.c
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月20日 星期四 23时14分22秒
 */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            keyboard.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ 

#include "../libc/types.h"
#include "../libc/string.h"
#include "keyboard.h"
#include "keymap.h"
#include "../cpu/isr.h" 
#include "../cpu/irq.h" 
#include "screen.h" 

#define KB_DATA 	0x60  
/*======================================================================*
                            keyboard_handler
 *======================================================================*/
void keyboard_handler(registers_t *regs)
{
	uint8 scan_code = inportb(KB_DATA);
	if (KB_INPUT.count < KB_IN_BYTES) {
		*(KB_INPUT.p_head) = scan_code;
		KB_INPUT.p_head++;
		if (KB_INPUT.p_head == KB_INPUT.buf + KB_IN_BYTES) {
			KB_INPUT.p_head = KB_INPUT.buf;
		}
		KB_INPUT.count++;
	}
}
/*======================================================================*
                           init_keyboard
*======================================================================*/
void init_keyboard()
{
	KB_INPUT.count = 0;
	KB_INPUT.p_head = KB_INPUT.p_tail = KB_IN.buf;

	register_interrupt_handler(IRQ1, keyboard_handler);	/* 设定键盘中断处理程序 */
}
/*======================================================================*
                           keyboard_read
*======================================================================*/
void keyboard_read()
{
	uint8	scan_code;
	char	output[2];
	t_bool	make;	/* TRUE : make  */
			/* FALSE: break */
	memory_set(output, 0, 2);
	if(KB_INPUT.count > 0){
		//disable_int();
		asm volatile ("cli"); 
		scan_code = *(KB_INPUT.p_tail);
		KB_INPUT.p_tail++;
		if (KB_INPUT.p_tail == KB_INPUT.buf + KB_IN_BYTES) {
			KB_INPUT.p_tail = KB_INPUT.buf;
		}
		KB_INPUT.count--;
		//enable_int();
		asm volatile ("sti"); 
		/* 下面开始解析扫描码 */
		if (scan_code == 0xE1) {
			/* 暂时不做任何操作 */
		}
		else if (scan_code == 0xE0) {
			/* 暂时不做任何操作 */
		}
		else {	/* 下面处理可打印字符 */
			
			/* 首先判断Make Code 还是 Break Code */
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
			/* 如果是Make Code 就打印，是 Break Code 则不做处理 */
			if(make){
				output[0] = keymap[(scan_code & 0x7F) * MAP_COLS];
				print(output);
			}
		}
	}
}
