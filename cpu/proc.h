/*
 *     File Name: proc.h
 *     Description: process header  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年09月01日 星期二 23时34分47秒
 */
#ifndef PROC_H 
#define PROC_H 

typedef struct s_stackframe 
{
	uint32 gs;  
	uint32 fs;  
	uint32 es;  
	uint32 ds;  
	uint32 edi;  
	uint32 esi;  
	uint32 ebp;  
	uint32 kernel_esp;  
	uint32 ebx;  
	uint32 edx;  
	uint32 ecx;  
	uint32 eax;  
	uint32 retaddr;  
	uint32 eip;  
	uint32 cs;  
	uint32 eflags;  
	uint32 esp;  
	uint32 ss;  
} STACK_FRAME; 

typedef struct s_proc
{
	STACK_FRAME regs;  	 	  	 	/* process reigisters saved in stack frame */ 
	uint16 ldy_sel; 	 	 	 	/* gdt selectoe givind ldt base and limit */ 
	DESCRIPTOR ldts[LDT_SIZE]; 	 	/* local descriptor for code and data */ 
	uint32 pid; 	 	 	 	 	/* process id passed in from MM */ 
	char p_name[16];  	 	 		/* name of the process */ 
}PROCESS; 

PROCESS proc_table[NR_TASKS]; 

#endif 
