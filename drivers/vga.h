/*
 *     File Name: vga.h
 *     Description: Video graphics Array  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.19 12:54:12
 */
#ifndef VGA_H 
#define VGA_H 

typedef 
enum real_color 
{
	rc_black = 0, 
	rc_blue = 1, 
	rc_green = 2, 
	rc_cyan = 3, 
	rc_red = 4, 
	rc_magenta = 5, 
	rc_brown = 6, 
	rc_light_grey = 7, 
	rc_dark_grey = 8, 
	rc_light_blue = 9, 
	rc_light_green = 10, 
	rc_light_cyan = 11, 
	rc_light_red = 12, 
	rc_light_magenta = 13, 
	rc_light_brown = 14,  	/* yellow */
	rc_white = 15 
} real_color_t; 

void clear_screen(); 

void putc_color(char c, real_color_t back, real_color_t fore); 

void printk(char *str); 

void printk_with_color(char *str, real_color_t back, real_color_t fore); 

#endif 
