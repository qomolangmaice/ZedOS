/*
 *     File Name: vga.c
 *     Description: Video Graphics Array  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.19 11:40:58
 */

#include "vga.h" 
#include "../libc/types.h" 
#include "../libc/system.h" 

/* VGA(Video graphics Array) is a video transmission standard by using analog signal.
 * kernel can through it to control the characters or graphics on the screen.
 * In the default Text Mode(Text-Mode), VGA controller keeps a piece of memory(0x8B00 ~ 0x8BFA0) as 
 * the character display on the screen of the buffer.
 * If you want to change the characters on the screen of the display, just modify that memory block */

static uint16 *video_memory = (uint16 *)0xB8000; 

/* Screen cursor location */
static uint8 cursor_x = 0; 
static uint8 cursor_y = 0; 

/* Move cursor */
static void move_cursor() 
{
	/* The width of screen is 80 byte */
	uint16 cursor_loc = cursor_y * 80 + cursor_x; 

	outportb(0x3D4, 14); 	 	 	 	 
	outportb(0x3D5, cursor_loc >> 8); 	/* High 8 bit */ 
	outportb(0x3D4, 15); 
	outportb(0x3D5, cursor_loc); 	 	/* Low 8 bit */ 

}

static void scroll() 
{
	/* attribute_byte is a description of black backgroud and white word */
	uint8 attribute_byte = (0 << 4) | (15 & 0x0F); 
	uint16 blank = 0x20 | (attribute_byte << 8); 	/* space is 0x20 */

	/* when cursor_y equals to 25, it means to jump to the next line */
	if (cursor_y >= 25) 
	{
		/* Copy all display data to the upper line, the first line is covered */
		int i; 
		for (i = 0 * 80; i < 24 * 80; i++) 
		{
			video_memory[i] = video_memory[i + 80]; 
		}

		/* Fill space in the end line, not display any character */
		for (i = 24 * 80; i < 25 * 80; i++) 
		{
			video_memory[i] = blank; 
		}

		/* Move up a line, now cursor_y equals to 24 */
		cursor_y = 24; 
	}
}

/* Clear the screen  */
void clear_screen() 
{
	uint8 attribute_byte = (0 << 4) | (15 & 0x0F); 
	uint16 blank = 0x20 | (attribute_byte << 8); 

	int i; 
	for (i = 0; i < 80 * 25; i++) 
	{
		video_memory[i] = blank; 
	}

	cursor_x = 0; 
	cursor_y = 0; 
	move_cursor(); 
}

/* input a color character in screen */
void putc_color(char c, real_color_t back, real_color_t fore) 
{
	uint8 back_color = (uint8)back; 
	uint8 fore_color = (uint8)fore; 

	uint8 attribute_byte = (back_color << 4) | (fore_color & 0x0F); 
	uint16 attribute = attribute_byte << 8; 

	/* 0x08 is the ASCII code of Backspace key */
	/* 0x09 is the ASCII code of Tab key */
	if (c == 0x08 && cursor_x) 
	{
		cursor_x--; 
	}
	else if (c == 0x09) 
	{
		cursor_x = (cursor_x + 8) & ~(8 - 1); 
	}
	else if (c == '\r') 
	{
		cursor_x = 0; 
	}
	else if (c == '\n') 
	{
		cursor_x = 0; 
		cursor_y++; 
	}
	else if (c >= ' ') 
	{
		video_memory[cursor_y * 80 + cursor_x] = c | attribute; 
		cursor_x++; 
	}

	/* With 80 characters a line, if cursor_x = 80, jump to the next line. */
	if (cursor_x >= 80) 
	{
		cursor_x = 0; 
		cursor_y++; 
	}

	/* Scroll the screen if needed */
	scroll(); 
 	move_cursor(); 
}

void printk(char *str) 
{
	while (*str) 
		putc_color(*str++, rc_black, rc_white); 
}

void printk_with_color(char *str, real_color_t back, real_color_t fore) 
{
	while (*str) 
		putc_color(*str++, back, fore); 
}

/* Print an array */
void print_array(char *c) 
{
	int i = 0; 
	while (c[i]) 
	{
		putc_color(c[i++], rc_black, rc_white); 
	}
}



/* Print a hexadecimal */
void print_hex(uint32 n) 
{
	int32 tmp; 

	printk("0x"); 

	char noZeroes = 1; 

	int i; 
	for (i = 28; i > 0; i -= 4) 
	{
		tmp = (n >> i) & 0xF; 
		if (tmp == 0 && noZeroes != 0) 
		{
			continue; 
		}

		if (tmp >= 0xA) 
		{
			noZeroes = 0; 
			putc_color(tmp - 0xA + 'a', rc_black, rc_white); 
		}
		else 
		{
			noZeroes = 0; 
			putc_color(tmp + '0', rc_black, rc_white); 
		}
	}

	tmp = n & 0xF; 
	if (tmp >= 0xA) 
	{
		putc_color(tmp - 0xA + 'a', rc_black, rc_white); 
	}
	else 
	{
		putc_color(tmp + '0', rc_black, rc_white); 
	}
}

void print_dec(uint32 n) 
{
	if (n == 0) 
	{
		putc_color('0', rc_black, rc_white); 
		return; 
	}

	int32 acc = n; 
	char c[32]; 
	int i = 0;  
	while (acc > 0) 
	{
		c[i] = '0' + acc % 10; 
		acc /= 10; 
		i++; 
	}
	c[i] = 0; 

	char c2[32]; 
	c2[i--] = 0; 
	int j = 0; 
	while(i >= 0) 
	{
		c2[i--] = c[j++]; 
	}
	print_array(c2); 
}
