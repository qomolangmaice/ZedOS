#ifndef ISR_H 
#define ISR_H 

#include "../libc/types.h" 

// regiester type 
typedef struct {
	uint32 ds; 	 	 	// save the data descriptor of user in 'ds' register 
	uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; 	 	// pusha edi ~ eax 
	uint32 int_no, err_code; 	    // number of interrupts  	
	uint32 eip, cs, eflags, useresp, ss; 
} registers_t; 

void isr_install(); 
// call interrupt handler function   
void isr_handler(registers_t *regs); 

// declare interrupt handler function , 0-19 belongs to the error interrupt of CPU 
// ISR: interrupt service routine 
extern void isr0(); 		
extern void isr1(); 		
extern void isr2(); 		
extern void isr3(); 		
extern void isr4(); 		
extern void isr5(); 		
extern void isr6(); 		
extern void isr7(); 		
extern void isr8(); 		
extern void isr9(); 		
extern void isr10(); 		
extern void isr11(); 		
extern void isr12(); 		
extern void isr13(); 		
extern void isr14(); 		
extern void isr15(); 		
extern void isr16(); 		
extern void isr17(); 		
extern void isr18(); 		
extern void isr19(); 		

// 20-31: Intel keeped  
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// 32～255: user custom error interrupt  
//extern void isr255();


#endif 
