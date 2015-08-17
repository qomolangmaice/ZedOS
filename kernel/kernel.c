#include "../drivers/keyboard.h" 
#include "../drivers/screen.h" 
#include "../cpu/gdt.h" 
#include "../cpu/idt.h" 
#include "../cpu/timer.h" 

kmain()
{
 	clear_screen();
	print("Initializing GDT ...");  
	gdt_install();  
	print("\nInitializing IDT ...");  
	idt_install(); 

	//asm volatile ("int $0x3"); 
	//asm volatile ("int $0x4"); 

	print_with_color("\nWelcome to Zed Operating System!", rc_red); 
	print_with_color("\nPlease enter a command.\n", rc_red); 

	init_timer(200);

	/* Open interrupt */
	asm volatile ("sti"); 

	/*
	while(1)
	{
	 	print("\nZedOS> "); 
		string ch = read_str(); 
		print("\n"); 
		print_with_color(ch, rc_green); 

		if(str_equal(ch, "cmd"))
		{
			print("\nYou are already in CMD.\n"); 
		}
		else if(str_equal(ch, "clear"))
		{
			clear_screen(); 
		}
		else 
		{
			print("\nBad Command\n"); 
		}

		print("\n"); 
	}
	*/ 
	return 0; 
}
