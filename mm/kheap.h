/*
 *     File Name: kheap.h
 *     Description: Interface for kernel heap functions, also 
 *      	 	 	a placement malloc() for use before the heap is initialised. 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.13 15:40:39
 */
#ifndef KHEAP_H 
#define KHEAP_H 

#include "../libc/types.h" 

/* Allocate a chunk of memory, sz in size. 
 * if align == 1, the chunk must be page-aligned.
 * If phys != 0, the physical location of the allocated chunk 
 * will be stored into phys. 
 *
 * This is the internal version of kmalloc. More user-friendly 
 * parameter representations are available in 
 * kmalloc, kmalloc_a, kmalloc_ap, kmalloc_p. 
 */

uint32 kmalloc_int(uint32 sz, int align, uint32 *phys); 

/* Allocate a chunk of memory, sz insize. The chunk must be page aligned. */
uint32 kmalloc_a(uint32 sz); 

/* Allocate a chunk of memory, sz in size. The physical address is 
 * returned in phys. Phys MUST be a valid pointer to uint32 !*/
uint32 kmalloc_p(uint32 sz, uint32 *phys); 

/* Allocate a chunk of memory , sz in size, The physical address is 
 * returned in phys. It muse be page-aligned.*/
uint32 kmalloc_ap(uint32 sz, uint32 *phys); 

/* General allocation function. */
uint32 kmalloc(uint32 sz); 

#endif 	
