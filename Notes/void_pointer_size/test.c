/*
 *     Question: Why a void pointer size is 4 bytes in 32 bit system ? 
 *     File Name: test.c
 *     Description: void pointer size test  
 *     Compile: 
 *     	 	 	32 bit system: cc test.c 
 *     	 	 	64 bit system: cc -m32 test.c(if not including '-m32' parameter, void pointer size is 8 bytes)  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.10.13 23:37:09
 */
#include <stdio.h> 

void main(int argc, char*argv[]) 
{
	typedef void* type_t; 

	printf("%d\n", sizeof(type_t)); 
}
