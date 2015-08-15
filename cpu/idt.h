#ifndef IDT_H 
#define IDT_H
#include "../libc/types.h" 

/* Defines an IDT entry */
typedef struct idt_entry_struct 
{
 	uint16 base_low; 
	uint16 selector; 
	uint8 always0;  
	uint8 flags; 
	uint16 base_high; 
} __attribute__((packed)) idt_entry_t;  

typedef struct idt_pointer_struct 
{
	uint16 limit; 
	uint32 base; 
} __attribute__((packed)) idt_ptr_t; 

static struct {
	idt_entry_t entries[256]; 
	idt_ptr_t 	pointer; 
} idt __attribute__((used));

extern void idt_load(uint32); 

void idt_set_gate(
		uint8 num,
		uint32 base, 
		uint16 selector, 
		uint8 flags); 

void idt_install(); 

#endif 
