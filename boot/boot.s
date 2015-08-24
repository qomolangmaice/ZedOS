;
; loader.asm -- Kernel start location. Also defines multiboot header.
; Based on Bran's kernel development tutorial file start.asm
;

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; All instructions should be 32-bit.

section .text 

dd  MBOOT_HEADER_MAGIC        ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
dd  MBOOT_CHECKSUM            ; To ensure that the above values are correct
   
[GLOBAL start] 
[GLOBAL glb_mboot_ptr]
[EXTERN kmain]                   ; This is the entry point of our C code

start:
	cli                         ; Disable interrupts.
	mov esp, STACK_TOP 
	mov ebp, 0 

	and esp, 0FFFFFF0H 
	mov [glb_mboot_ptr], ebx 
	call kmain                  ; call our kmain() function.

stop: 
	hlt 
	jmp stop 

section .bss 

stack: 
	resb 32768 

glb_mboot_ptr: 
	resb 4 

STACK_TOP equ $-stack-1 
