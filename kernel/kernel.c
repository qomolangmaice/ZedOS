#include "../drivers/keyboard.h" 
#include "../drivers/screen.h" 
#include "../cpu/gdt.h" 
#include "../cpu/idt.h" 
#include "../cpu/timer.h" 
#include "../mm/multiboot.h" 
#include "../mm/pmm.h" 

kmain()
{
	//clear_screen(); 
	print_with_color("\nWelcome to Zed Operating System!", rc_red); 
	print_with_color("\nPlease enter a command.\n", rc_red); 
	print("\n"); 

	print("Initializing GDT ...");  
	gdt_install();  
	print("\nInitializing IDT ...\n");  
	idt_install(); 

	//init_timer(200); 
	/* Test for keyboard */
	init_keyboard(); 
	asm volatile ("sti"); 
	for(;;); 
	
 /*	
	printf("kernel in memory start: 0x%08x\n", kern_start); 
	printf("kernel in memory end: 0x%08x\n", kern_end); 
	printf("kernel in memory used: %d KB\n\n", (kern_end - kern_start) + 1023); 

	show_memory_map(); 

	return 0; 
	*/
}
