#include "../drivers/keyboard.h" 
#include "../drivers/screen.h" 
#include "../cpu/gdt.h" 
#include "../cpu/idt.h" 
#include "../cpu/timer.h" 
#include "../mm/multiboot.h" 
#include "../mm/pmm.h" 
#include "../drivers/tty.h" 

kmain()
{
	//clear_screen(); 
	
	print_with_color("\nThis is tty1.", rc_green); 
	print_with_color("\nWelcome to Zed Operating System!", rc_red); 
	print("\n"); 

	print("Initializing GDT ...");  
	gdt_install();  
	print("\nInitializing IDT ...\n");  
	idt_install(); 

	/* Test for TTY and consoles */
	task_tty(); 

	/* Test for timer */
	//init_timer(200); 
	
	/* Test for keyboard */
	//init_keyboard(); 
	//asm volatile ("sti"); 
	//for(;;); 
}
