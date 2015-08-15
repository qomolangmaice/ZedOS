#ifndef SCREEN_H 
#define SCREEN_H 

#include "../libc/types.h" 
#include "../libc/system.h" 
#include "../libc/string.h" 

#define  rc_balck         0x00 
#define  rc_blue 	 	  0x01 
#define  rc_green 	 	  0x02 
#define  rc_cyan 	 	  0x03  
#define  rc_red  	 	  0x04 
#define  rc_magenta  	  0x05 
#define  rc_brown  	 	  0x06 
#define  rc_light_grey    0x07   
#define  rc_dark_grey  	  0x08  
#define  rc_light_blue    0x09  
#define  rc_light_green   0x0A  
#define  rc_light_cyan    0x0B 
#define  rc_light_red     0x0C 
#define  rc_light_megenta 0x0D   
#define  rc_light_brown   0x0E 
#define  rc_light_white   0x0F   

// current cursor is located by cursorX and cursorY in the screen    
int cursor_x, cursor_y;    

// clear all lines from "startline" to "endline" in the screen    
void clear_line(uint8 startline, uint8 endline);  

// I'm puzzled with this function @_@   
void update_cursor();   

void clear_screen();   

void scroll_up(uint8 line_number);   

void newline_check();   

void printch(char c, uint8 font_color);   

void print_with_color(string str, uint8 font_color);   

void print(string str);   

#endif 
