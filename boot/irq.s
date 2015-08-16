;
;    File Name: irq.s  
;    Description: interrupt request handler  
;    Author: iczelion
;    Email: qomolangmaice@163.com 
;    Created: 2015年08月16日 星期日 18时45分14秒

; Build interrupt request macro 
%macro IRQ 2
[GLOBAL irq%1]
irq%1: 
	 	cli
		push 0 
		push %2 
		jmp irq_common_stub 
%endmacro 

IRQ   0, 32; 	
IRQ   1, 33; 	
IRQ   2, 34; 	
IRQ   3, 35; 	
IRQ   4, 36; 	
IRQ   5, 37; 	
IRQ   6, 38; 	
IRQ   7, 39; 	
IRQ   8, 40; 	
IRQ   9, 41; 	
IRQ  10, 42; 	
IRQ  11, 43; 	
IRQ  12, 44; 	
IRQ  13, 45; 	
IRQ  14, 46; 	
IRQ  15, 47; 	

[GLOBAL irq_common_stub] 
[EXTERN irq_handler] 
irq_common_stub: 
	pusha 	 	 	; Pushes edi, esi, ebp, ebx, edx, ecx, eax 
	mov ax, ds 
	push eax 	 	; save data segment descriptor 

	mov ax, 0x10 	; load kernel data segment descriptor 
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax 

	push esp  	 	 ; the value in 'esp' register is equal to the pointer of registers_t(in 'isr.c')  
	call irq_handler ; in other C code 
	add esp, 4  	 ; clean parameters that has been pushed 

	pop ebx 	 	 ; recover the original data segment descriptor 
	mov ds, bx 
	mov es, bx 
	mov fs, bx 
	mov gs, bx 
	mov ss, bx 

	popa 	 	 	; Pops edi, esi, ebp, ebx, edx, ecx, eax 
	add esp, 8 	 	; clean error code and ISR number in stack 
	iret 	 	 	; Pop CS, EIP, EFLAGS, SS, ESP  
.end: 

