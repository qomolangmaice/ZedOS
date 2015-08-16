
; 1.for interrupts without error code 
%macro ISR_NOERRCODE 1 
[GLOBAL isr%1] 	 	 	 	; %1 accesses the first parameter  
isr%1:
	cli						; close down interrupts 
	push byte 0				; push non-effective interrupt error code 
	push byte %1			; push the number of interrupt 
	jmp isr_commom_stub 
%endmacro 

; 2.for interrupts with error code 
%macro ISR_ERRCODE 1
[GLOBAL isr%1] 
isr%1:
	cli 	 	 	 	 	; close down interrupts 
	push byte %1 	 	 	; push the number of interrupt 
	jmp isr_commom_stub 
%endmacro 

; 3.define interrupt handler function 
ISR_NOERRCODE 0 
ISR_NOERRCODE 1 
ISR_NOERRCODE 2 
ISR_NOERRCODE 3 
ISR_NOERRCODE 4 
ISR_NOERRCODE 5 
ISR_NOERRCODE 6 
ISR_NOERRCODE 7 
ISR_ERRCODE   8 
ISR_NOERRCODE 9  
ISR_ERRCODE   10 
ISR_ERRCODE   11 
ISR_ERRCODE   12 
ISR_ERRCODE   13 
ISR_ERRCODE   14 
ISR_NOERRCODE 15  
ISR_NOERRCODE 16  
ISR_ERRCODE   17  
ISR_NOERRCODE 18  
ISR_NOERRCODE 19  

; 20~31 interrupts keeped by Intel  
ISR_NOERRCODE 20  
ISR_NOERRCODE 21  
ISR_NOERRCODE 22  
ISR_NOERRCODE 23  
ISR_NOERRCODE 24  
ISR_NOERRCODE 25  
ISR_NOERRCODE 26  
ISR_NOERRCODE 27  
ISR_NOERRCODE 28  
ISR_NOERRCODE 29  
ISR_NOERRCODE 30  
ISR_NOERRCODE 31  

; 32 ~ 255 User custom 
; ISR_NOERRCODE 255 

; 4.Interrupt service handler 
[extern isr_handler] 
isr_commom_stub: 
	pusha 	 	 	; Pushes edi, esi, ebp, ebx, edx, ecx, eax 
	mov ax, ds 
	push eax 	 	; save data segment descriptor 

	mov ax, 0x10 	; load kernel data segment descriptor 
	mov ds, ax 
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax 

	push esp  	 	; the value in 'esp' register is equal to the pointer of pt_regs_t 
	call isr_handler 	; in other C code 
	add esp, 4  	; clean parameters that has been pushed 

	pop ebx 	 	; back to the orignal data segment descriptor 
	mov ds, bx 
	mov es, bx 
	mov fs, bx 
	mov gs, bx 
	mov ss, bx 

	popa 	 	 	; Pops edi, esi, ebp, ebx, edx, ecx, eax 
	add esp, 8 	 	; clean error code and ISR in stack 
	iret 
.end: 

