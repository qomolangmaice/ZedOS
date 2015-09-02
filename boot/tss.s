;
;    File Name: tss.s  
;    Description: Task Status Segment  
;    Author: iczelion
;    Email: qomolangmaice@163.com 
;    Created: 2015.09.03 Thus 00:48:51 

global tss_flush 

tss_flush: 
	mov ax, 0x2B 
	ltr ax 
	ret 



