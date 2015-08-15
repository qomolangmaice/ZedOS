
global idt_load 
idt_load: 
 	mov eax, [4 + esp] 
	lidt [eax] 
	ret 

