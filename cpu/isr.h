#ifndef ISR_H 
#define ISR_H 

#include "../libc/types.h" 
#include "../libc/system.h" 

// regiester type 
typedef struct protected_register_struct {
	uint32 ds; 	 	 	// save the data descriptor of user in 'ds' register 
	uint32 edi, esi, ebp, ebx, edx, ecx, eax; 	 	// pusha edi ~ eax 
	uint32 int_no, err_code; 	    // number of interrupts  	
	uint32 eip, cs, eflags, useresp, ss; 
} pt_regs_t; 

// define interrupt handler function pointer 
typedef void (*interrupt_handler_t)(pt_regs_t *); 

// interrupt handler function pointer array 
interrupt_handler_t interrupt_handlers[256]; 

// regest an interrupt function 
void register_interrupt_handler(uint8 num, interrupt_handler_t handler); 

// call interrupt handler function   
void isr_handler(pt_regs_t *regs); 

void isr_memset(); 
void isr_init(); 

// 声明中断处理函数 0-19 属于 CPU 的异常中断
// ISR:中断服务程序(interrupt service routine)
void isr0(); 		// 0 #DE 除 0 异常 
void isr1(); 		// 1 #DB 调试异常 
void isr2(); 		// 2 NMI 
void isr3(); 		// 3 BP 断点异常 
void isr4(); 		// 4 #OF 溢出 
void isr5(); 		// 5 #BR 对数组的引用超出边界 
void isr6(); 		// 6 #UD 无效或未定义的操作码 
void isr7(); 		// 7 #NM 设备不可用(无数学协处理器) 
void isr8(); 		// 8 #DF 双重故障(有错误代码) 
void isr9(); 		// 9 协处理器跨段操作 
void isr10(); 		// 10 #TS 无效TSS(有错误代码) 
void isr11(); 		// 11 #NP 段不存在(有错误代码) 
void isr12(); 		// 12 #SS 栈错误(有错误代码) 
void isr13(); 		// 13 #GP 常规保护(有错误代码) 
void isr14(); 		// 14 #PF 页故障(有错误代码) 
void isr15(); 		// 15 CPU 保留 
void isr16(); 		// 16 #MF 浮点处理单元错误 
void isr17(); 		// 17 #AC 对齐检查 
void isr18(); 		// 18 #MC 机器检查 
void isr19(); 		// 19 #XM SIMD(单指令多数据)浮点异常

// 20-31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32～255 用户自定义异常
void isr255();


#endif 
