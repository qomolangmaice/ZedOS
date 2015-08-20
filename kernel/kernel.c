#include "../drivers/keyboard.h" 
#include "../drivers/screen.h" 
//#include "gdt.h" 
//#include "idt.h" 
//#include "timer.h" 

kmain()
{
 	clear_screen();
	print_with_color("\nWelcome to Zed Operating System!", rc_red); 
	print_with_color("\nPlease enter a command.\n", rc_red); 
	print("\n"); 

	print("Initializing GDT ...");  
	gdt_install();  
	print("\nInitializing IDT ...\n");  
	idt_install(); 

	init_keyboard(); 

	while(1)
	{
		keyboard_read(); 
	}
}
