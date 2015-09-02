/*
 *     File Name: tss.h
 *     Description: Task Status Segment  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年09月02日 星期三 21时03分03秒
 */
#ifndef TSS_H 
#define TSS_H 
#include "../libc/types.h" 

extern void tss_flush(void); 

typedef struct tss_entry 
{
	uint32 prev_tss; 	/* Previous tss */ 
	uint32 esp0; 	 	/* stack pointer use during interrupt */ 
	uint32 ss0; 
	uint32 esp1; 
	uint32 ss1; 
	uint32 esp2; 
	uint32 ss2; 
	uint32 cr3; 
	uint32 eip; 
	uint32 flags; 
	uint32 eax; 
	uint32 ecx; 
	uint32 edx; 
	uint32 ebx; 
	uint32 esp; 
	uint32 ebp; 
	uint32 esi; 
	uint32 edi; 
	uint32 es; 
	uint32 cs; 
	uint32 ss; 
	uint32 ds; 
	uint32 fs; 
	uint32 gs; 
	uint32 ldt; 
	uint32 trap; 
	uint32 iomap_base; 
} __attribute__ ((packed)) tss_entry_t; 

#endif  
