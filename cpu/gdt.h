#ifndef GDT_H 
#define GDT_H 
#include "../libc/types.h" 
#include "tss.h" 

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure. 
typedef struct gdt_entry_struct 
{
	/* Limits */
	uint16 limit_low;					// The lower 16 bits of the limit  
	/* Segment address */
	uint16 base_low;					// THe lower 16 bits of the base  
	uint8 base_middle;					// The next 8 bits of the base.  
	/* Access modes */
	uint8 access;						// Access flags, determine what ring this segmeny can be used in.  
	uint8 granularity; 
	uint8 base_high;					// The last 8 its of the base  
} __attribute__((packed)) gdt_entry_t; 

typedef struct gdt_pointer_struct{
	uint16 limit;						// The upper 16 bits of all selector limits  
	uint32 base;						// The address of the first gdt_entry_t struct  
} __attribute__((packed)) gdt_ptr_t;  


static struct {
 	gdt_entry_t entries[6]; 
	gdt_ptr_t  	pointer; 
	tss_entry_t tss; 
} gdt __attribute__((used)); 

/*  function 'gdt_flush()' is defined in /boot/boot.s   */
extern void gdt_flush(uint32); 

static void gdt_set_gate(
		int32 num, 
		uint32 base, 
	 	uint32 limit, 
		uint8 access, 
		uint8 gran);    

void gdt_install(); 

#endif  
