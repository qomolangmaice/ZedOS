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

void TestA() 
{
	int i = 0; 
	while(1)
	{
		print("A."); 
		printf("%d", i++); 
		delay(1); 
	}
}

void delay(int time) 
{
	int i, j, k; 
	for(k=0; k<time; k++)
	{
		for(i=0; i<10; i++)
		{
			for(j=0; j<10000; j++) {} 
		}
	}
}
