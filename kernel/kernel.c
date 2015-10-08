#include "../drivers/keyboard.h" 
#include "../cpu/gdt.h" 
#include "../cpu/idt.h" 
#include "../cpu/timer.h" 
#include "../libc/printf.h" 
#include "../mm/paging.h" 
#include "../mm/kheap.h" 
#include "../drivers/tty.h" 

kmain()
{
	clear_screen(); 
	printf_with_color(rc_black, rc_green, "This is tty1.\n"); 
	printf_with_color(rc_black, rc_red, "Welcome to Zed Operating System! ^_^\n"); 

	printf("\nInitializing GDT ...\n");  
	gdt_install();  

	printf("Initializing IDT ...\n");  
	idt_install(); 

	uint32 a = kmalloc(8); 
 	initialise_paging(); 
	printf("\nSetup Paging over. :)\n");  
	//print_directory(); 
	uint32 b = kmalloc(8); 
	uint32 c = kmalloc(8); 
	printf("a: "); 
	print_hex(a); 
	printf(", b: "); 
	print_hex(b); 
	printf("\nc: "); 
	print_hex(c); 

	kfree(c); 
	kfree(b); 
	uint32 d = kmalloc(12); 
	printf(", d: "); 
	print_hex(d); 

	/* Test divide zero error interrupt */
	//int i = 5/0; 
	
	/* Test for page fault */
	//uint32 *ptr = (uint32 *)0xA0000000; 
	//uint32 do_page_fault = *ptr; 

	/* Test for TTY and consoles */
	//task_tty(); 


	/* Test divide zero error interrupt */
	//int i = 5/0; 

	/* Test printf function */
	//printf("\n%s--This is No.%d Test printf in address 0x%X.:)\n", "Yep", 1, 0xFFFFFFFF);  

	/* Test for timer */
	//init_timer(200); 
	
	/* Test for keyboard */
	//init_keyboard(); 
	//asm volatile ("sti"); 
	//for(;;); 
	
	return 0; 
}

