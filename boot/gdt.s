
global gdt_flush				 ; permit C code link to here 

gdt_flush: 
	mov eax, [esp + 4] 	 	 	 ; Get the pointer to the GDT, passed as a parameter   
	lgdt [eax] 			 	 	 ; load the new GDT pointer   

	mov ax, 0x10				 ; 0x10 is the offset in the GDT to data segment in GDT 
	mov ds, ax 	 	 	 	 	 ; Load all data segment selectors  
	mov es, ax 
	mov fs, ax 
	mov gs, ax 
	mov ss, ax 

	jmp 0x08:.flush				 ; 0x80 is the offset to code segment.Far jump!    

.flush: 
	ret							 ; return C code 

