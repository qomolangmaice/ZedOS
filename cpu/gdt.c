#include "gdt.h" 
#include "../libc/types.h" 
#define ENTRY(X) (gdt.entries[(X)]) 

void gdt_set_gate(
		int32 num, 
		uint32 base, 
		uint32 limit, 
		uint8 access, 
		uint8 gran) 
{
	/* Base Address */
 	ENTRY(num).base_low = (base & 0xFFFF); 
	ENTRY(num).base_middle = (base >> 16) & 0xFF; 
	ENTRY(num).base_high = (base >> 24) & 0xFF; 

	/* Limits */
	ENTRY(num).limit_low = (limit & 0xFFFF); 
	ENTRY(num).granularity = (limit >> 16) & 0x0F; 

	/* Granularity */
	ENTRY(num).granularity |= (gran & 0xF0); 

	/* Access flags */
	ENTRY(num).access = access; 
}

void gdt_install()
{
	gdt_ptr_t *gdtp = &gdt.pointer; 
	gdtp->limit = (sizeof(struct gdt_entry_struct) * 5) - 1; 
	gdtp->base = (uint32)&ENTRY(0); 

	gdt_set_gate(0, 0, 0, 0, 0); 	 	 	 	 	// NULL segment  
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  	// Code segment   
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   	// Data segment  
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  	// User mode code segment  
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  	// User mode data segment  

	gdt_flush((uint32)gdtp);  
}
