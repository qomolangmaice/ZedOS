/*
 *     File Name: irq.c
 *     Description: interrupt requests handler  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月16日 星期日 16时24分53秒
 */
#include "irq.h" 
#include "idt.h" 
#include "isr.h" 

/*
 * Normally, IRQs 0 to 7 are remapped to entries 8 to 15.
 * This is a problem i protected mode, because IDT entry 8 is a
 * Doule Fault! Without remapping, every time IRQs fires, 
 * you get a Double Fault Exception, which is NOT actually 
 * what's happenning. We send commands to the Programmable 
 * Interrupt Conroller (PICs -also called the 8259's) in 
 * order to make IRQ0 to IRQ15 be remapped to IDT entries 32 to 47 
 * 
 */  
void irq_remap()
{

	/* Remap IRQ table
	 * Intel 8259A chip(Master control chip, Slave control chip)  
	 * Master control chip port: 0x20 0x21
	 * Slave control chip port: 0xA0 0xA1
 	 */	

	/* intializing master control chip and slave control chip */  
 	outportb(0x20, 0x11); 
 	outportb(0xA0, 0x11); 

	/* setup master control chip's IRQ to start from No.32(0x20) interrupt  */
	outportb(0x21, 0x20); 

	/* setup slave control chip's IRQ to start from No.40(0x28) interrupt  */
 	outportb(0xA1, 0x28); 

	/* setup IR2 pin which is in master control chip connected from slave control chip */
 	outportb(0x21, 0x04); 

	/* setup slave control chip connected from IR2 pin which is in master control chip  */
 	outportb(0xA1, 0x02); 

	/* setup master control chip and slave control chip work in 8086 mode */
	outportb(0x21, 0x01); 
 	outportb(0xA1, 0x01); 

	/* setup master control chip and slave control chip allow interrupt handler */
 	outportb(0x21, 0x0); 
 	outportb(0xA1, 0x0); 
}

/*
 * We first remap the interrupt controls, and then we install 
 * the appropriate ISRs to the correct entries int the IDT.
 * This is just like installing the exception handlers.
 */
void irq_install()
{
	irq_remap(); 

	idt_set_gate(32, (uint32)irq0, 0x08, 0x8E); 
	idt_set_gate(33, (uint32)irq1, 0x08, 0x8E); 
	idt_set_gate(34, (uint32)irq2, 0x08, 0x8E); 
	idt_set_gate(35, (uint32)irq3, 0x08, 0x8E); 
	idt_set_gate(36, (uint32)irq4, 0x08, 0x8E); 
	idt_set_gate(37, (uint32)irq5, 0x08, 0x8E); 
	idt_set_gate(38, (uint32)irq6, 0x08, 0x8E); 
	idt_set_gate(39, (uint32)irq7, 0x08, 0x8E); 

	idt_set_gate(40, (uint32)irq8, 0x08, 0x8E); 
	idt_set_gate(41, (uint32)irq9, 0x08, 0x8E); 
	idt_set_gate(42, (uint32)irq10, 0x08, 0x8E); 
	idt_set_gate(43, (uint32)irq11, 0x08, 0x8E); 
	idt_set_gate(44, (uint32)irq12, 0x08, 0x8E); 
	idt_set_gate(45, (uint32)irq13, 0x08, 0x8E); 
	idt_set_gate(46, (uint32)irq14, 0x08, 0x8E); 
	idt_set_gate(47, (uint32)irq15, 0x08, 0x8E); 
}

void register_interrupt_handler(uint8 irq_no, interrupt_handler_ptr handler)
{
	interrupt_handlers[irq_no] = handler; 
}

/* Interrupt handler function */
void irq_handler(registers_t *regs)
{
	/*
	 * If the IDT entry that was invoked was greater than 40 
	 * (meaning IRQ8 - IRQ15), then we need to send an EOI to 
	 * slave control chip 
	 */ 
	if(regs->int_no >= 40) 
		outportb(0xA0, 0x20); 

	outportb(0x20, 0x20); 

	/* This is a blank function pointer */
	//void (*handler)(registers_t *regs); 
	interrupt_handler_ptr handler; 

 	handler = interrupt_handlers[regs->int_no];  

	if(handler)
	{
		handler(regs); 
	}
}
