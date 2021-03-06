/*
 *     File Name: irq.h
 *     Description: IRQs headr file  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月16日 星期日 16时40分59秒
 */
#ifndef IRQ_H 
#define IRQ_H 
#include "../libc/types.h" 
#include "isr.h" 

#define IRQ0   32 
#define IRQ1   33 
#define IRQ2   34 
#define IRQ3   35 
#define IRQ4   36 
#define IRQ5   37 
#define IRQ6   38 
#define IRQ7   39 
#define IRQ8   40 
#define IRQ9   41 
#define IRQ10  42 
#define IRQ11  43 
#define IRQ12  44 
#define IRQ13  45 
#define IRQ14  46 
#define IRQ15  47 

// IRQ definitions 
extern void irq0(); 
extern void irq1(); 
extern void irq2(); 
extern void irq3(); 
extern void irq4(); 
extern void irq5(); 
extern void irq6(); 
extern void irq7(); 

extern void irq8(); 
extern void irq9(); 
extern void irq10(); 
extern void irq11(); 
extern void irq12(); 
extern void irq13(); 
extern void irq14(); 
extern void irq15(); 

void irq_remap(); 
void irq_install(); 

/* Interrupt request handler function */
void irq_handler(registers_t regs);  
void register_interrupt_handler(uint8 irq_no, interrupt_handler_ptr handler); 

#endif 
