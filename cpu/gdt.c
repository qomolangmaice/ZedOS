#include "gdt.h" 
#include "../libc/types.h" 
#include "tss.h" 
#include "../libc/string.h" 
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

static void write_tss(int32 num, uint16 ss0, uint32 esp0); 

void gdt_install()
{
	gdt_ptr_t *gdtp = &gdt.pointer; 
	gdtp->limit = (sizeof(struct gdt_entry_struct) * 6) - 1; 
	gdtp->base = (uint32)&ENTRY(0); 

	gdt_set_gate(0, 0, 0, 0, 0); 	 	 	 	 	// NULL segment  
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  	// Code segment   
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   	// Data segment  
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  	// User mode code segment  
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  	// User mode data segment  
	write_tss(5, 0x10, 0x0); 

	gdt_flush((uint32)gdtp);  
	tss_flush(); 
}

static void write_tss(int32 num, uint16 ss0, uint32 esp0) 
{
	tss_entry_t *tss = &gdt.tss; 
	uint32 base = (uint32)tss; 
	uint32 limit = base + sizeof *tss; 

	/* Add the TSS descriptor to the GDT */
	gdt_set_gate(num, base, limit, 0xE9, 0x00); 

	memory_set(tss, 0x0, sizeof *tss); 

	tss->ss0 = ss0; 
	tss->esp0 = esp0; 
	tss->cs = 0x0b; 
	tss->ss = 0x13; 
	tss->ds = 0x13; 
	tss->es = 0x13; 
	tss->fs = 0x13; 
	tss->gs = 0x13; 

	tss->iomap_base = sizeof *tss; 
}

void set_kernel_stack(uint32 stack) 
{
	/* Set the kernel stack */
	gdt.tss.esp0 = stack; 
}
