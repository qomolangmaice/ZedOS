;
;    File Name: proc.s  
;    Description: 
;    Author: iczelion
;   Email: qomolangmaice@163.com 
;   Created: 2015.09.04  00:32:05

P_STACKBASE	equ	0
GSREG		equ	P_STACKBASE
FSREG		equ	GSREG		+ 4
ESREG		equ	FSREG		+ 4
DSREG		equ	ESREG		+ 4
EDIREG		equ	DSREG		+ 4
ESIREG		equ	EDIREG		+ 4
EBPREG		equ	ESIREG		+ 4
KERNELESPREG	equ	EBPREG		+ 4
EBXREG		equ	KERNELESPREG	+ 4
EDXREG		equ	EBXREG		+ 4
ECXREG		equ	EDXREG		+ 4
EAXREG		equ	ECXREG		+ 4
RETADR		equ	EAXREG		+ 4
EIPREG		equ	RETADR		+ 4
CSREG		equ	EIPREG		+ 4
EFLAGSREG	equ	CSREG		+ 4
ESPREG		equ	EFLAGSREG	+ 4
SSREG		equ	ESPREG		+ 4
P_STACKTOP	equ	SSREG		+ 4
P_LDT_SEL	equ	P_STACKTOP
P_LDT		equ	P_LDT_SEL	+ 4

TSS3_S_SP0	equ	4

SELECTOR_FLAT_C		equ		0x08	 	 	 	; In LOADER
SELECTOR_TSS		equ		0x20 	 	 		; TSS
SELECTOR_KERNEL_CS	equ		SELECTOR_FLAT_C


global restart 
extern p_proc_ready 
extern tss 

restart: 
	mov esp, [p_proc_ready] 	 	 	; Next process address that will be started  
	lldt [esp + P_LDT_SEL] 
	lea eax, [esp, P_STACKTOP] 
	mov dword [tss + TSS3_S_SP0], eax 

	pop gs 
	pop fs 
	pop es 
	pop ds 
	popad 
	add esp, 4 
	iretd 

