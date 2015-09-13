#include "../drivers/keyboard.h" 
#include "../drivers/screen.h" 
#include "../cpu/gdt.h" 
#include "../cpu/idt.h" 
#include "../cpu/timer.h" 
#include "../mm/paging.h" 
#include "../drivers/tty.h" 

kmain()
{
	//clear_screen(); 
	
	print_with_color("\nThis is tty1.", rc_green); 
	print_with_color("\nWelcome to Zed Operating System! ^_^", rc_red); 
	print("\n"); 

	print("Initializing GDT. :P");  
	gdt_install();  
	print("\nInitializing IDT. :P\n");  
	idt_install(); 

 	initialise_paging(); 
	print("\nSetup Paging over. :)\n");  

	/* Test for TTY and consoles */
	task_tty(); 

	//int i = 5/0; 

	//uint32 *ptr = (uint32 *)0xA0000000; 
	//uint32 do_page_fault = *ptr; 

	/* Test for timer */
	//init_timer(200); 
	
	/* Test for keyboard */
	//init_keyboard(); 
	//asm volatile ("sti"); 
	//for(;;); 
	
	return 0; 
}

