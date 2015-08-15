#include "idt.h" 
#include "isr.h" 
#include "../libc/types.h" 
#include "../libc/system.h" 
#include "../libc/string.h" 
#include "../libc/printk/debug.h"   

#define ENTRY(X) (idt.entries[(X)])  

void idt_set_gate(
		uint8 num, 
		uint32 base, 
		uint16 selector, 
		uint8 flags) 
{
	ENTRY(num).base_low = (base & 0xFFFF); 
	ENTRY(num).base_high = (base >> 16) & 0xFFFF; 
	ENTRY(num).selector = selector; 
	ENTRY(num).always0 = 0; 
	ENTRY(num).flags = flags; 
}

void idt_install() 
{
	/* clear out the entire ISR, initializing it to zeros */
	isr_memset(); 

	idt_ptr_t *idtp = &idt.pointer;  
 	idtp->limit = (sizeof(struct idt_entry_struct) * 256) - 1; 
	idtp->base = (uint32)&ENTRY(0); 

	/* clear out the entire IDT, initializing it to zeros */
	memory_set(&ENTRY(0), 0, sizeof(struct idt_entry_struct)*256); 

	// setup Interrupt Service Routines, in isr.c 
	isr_init(); 

	/* points to the processor's internal register to the new IDT */
	idt_load((uint32)idtp); 
}
