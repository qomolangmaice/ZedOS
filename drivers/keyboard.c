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

#define KB_DATA  0x60  

KB_INPUT kbinput; 
/*======================================================================*
                           keyboard_read
*======================================================================*/
void keyboard_read()
{
	uint8	scan_code;
	char	output[2];
 	uint8	make;	/* TRUE : make  */
			/* FALSE: break */
	memory_set(output, 0, 2);
	if(kbinput.count > 0){
		//disable_int();
		//asm volatile ("cli"); 
		scan_code = *(kbinput.p_tail);
		kbinput.p_tail++;
		if (kbinput.p_tail == kbinput.buf + KB_IN_BYTES) {
			kbinput.p_tail = kbinput.buf;
		}
		kbinput.count--;
		//enable_int();
		//asm volatile ("sti"); 
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

/*======================================================================*
                            keyboard_handler
 *======================================================================*/
void keyboard_handler(registers_t *regs)
{
	uint8 scan_code = inportb(KB_DATA);
	if (kbinput.count < KB_IN_BYTES) {
		*(kbinput.p_head) = scan_code;
		kbinput.p_head++;
		if (kbinput.p_head == kbinput.buf + KB_IN_BYTES) {
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
	kbinput.count = 0;
	kbinput.p_head = kbinput.p_tail = kbinput.buf;

	register_interrupt_handler(IRQ1, keyboard_handler);	/* 设定键盘中断处理程序 */
}

