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
#define IRQ16  48 

void irq_remap(void); 
void irq_install(); 

/* This is a blank interrupt handler function pointer */
/* registers_t is in 'isr.h' */
typedef void (*interrupt_handler_ptr)(registers_t *); 
/* Interrupt handler function pointer array */
interrupt_handler_ptr interrupt_handlers[256]; 

void register_interrupt_handler(uint8 irq_no, interrupt_handler_ptr handler); 

#endif 
