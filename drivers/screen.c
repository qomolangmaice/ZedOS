#include "screen.h" 

// current cursor is located by cursor_x and cursor_y in the screen    
int cursor_x = 0;    
int cursor_y = 0;          		

// screen width = 80, screen height = 25, screen depth = 2;  

// clear all lines from "startline" to "endline" in the screen    
void clear_line(uint8 startline, uint8 endline)
{
 	uint16 i = 80 * startline * 2; 
	// 0xB8000 is the start address of VGA text color(0xb8000~0xbFFFF=32KB)   
 	// vidmem means the start point of video card memory address   
	string vidmem = (string)0xB8000;  	 	

	// clear all charactors one by one in every line of the range(startline, endline) 
 	for(i; i<(80*(endline+1)*2); i++)
		vidmem[i] = 0x0; 
}

void update_cursor()
{
 	unsigned cursor_loc; 

	cursor_loc = cursor_y * 80 + cursor_x;    // Cursor Position = (y * width) + x  

	outportb(0x3D4, 14);					  // CRT control Register: Select Cursor Location  
	outportb(0x3D5, cursor_loc >> 8);		  // Send the high byte across the bus  
	outportb(0x3D4, 15);					  // CRT control Register: Select Send Low Byte  
	outportb(0x3D5, cursor_loc);			  // Send the low byte of the cursor location    
}


void clear_screen()
{
 	clear_line(0, 25-1); 
	cursor_x = 0; 
	cursor_y = 0; 
	update_cursor(); 
}

void scroll_up(uint8 line_number) 
{
 	string vidmem = (string)0xB8000; 
	uint16 i = 0; 

	// refresh line number 
	for(i; i<80*(25-1)*2; i++)
		vidmem[i] = vidmem[i + 80*2*line_number]; 

	// delete all lines before linenumber   
	clear_line(0,0); 

	if((cursor_y - line_number)<0)
	{
 	 	cursor_x = 0; 
		cursor_y = 0; 
	}
	else
	{
 	 	cursor_y -= line_number; 	
	}
	
	update_cursor(); 
}

void newline_check()
{
	if(cursor_y >= 25-1)
		scroll_up(1); 
}

void printch(char c, uint8 font_color)
{
 	string vidmem = (string)0xb8000; 

	switch(c)
	{
		 case (0x08): 
			if(cursor_x > 0) 
			{
				cursor_x--; 
				vidmem[(cursor_y * 80 + cursor_x)*2] = 0x00;  
			}
			break; 
		 case (0x09): 
			cursor_x = (cursor_x + 8) & ~(8 - 1); 
			break; 
		 case ('\r'): 
			cursor_x = 0; 
			break; 
		 case ('\n'): 
			cursor_x = 0; 
			cursor_y++; 
			break; 
		 default: 
			vidmem[((cursor_y * 80 + cursor_x))*2] = c; 
			// setup screen font color 
			vidmem[((cursor_y * 80 + cursor_x))*2 + 1] = font_color;   
			cursor_x++; 
			break;  
	}
 	
	// if cursor_x is in the end of the current line, let cursor jump to next line 
	if(cursor_x >= 80)
	{
 	 	cursor_x = 0; 
		cursor_y++; 
	}
	newline_check(); 
	update_cursor(); 
}

void print_with_color(string str, uint8 font_color)
{
 	uint16 count = 0; 
	
	/*
	for(count; count<strlen(str); count++)
		printch(ch[count], font_color);   
	*/
	while(count < str_len(str) - 1){
		printch(str[count], font_color); 
		count++; 
	} 
}

void print(string str) 
{
 	uint16 count = 0; 

	while(count < str_len(str) - 1){
		printch(str[count], rc_blue); 
		count++; 
	} 
}
