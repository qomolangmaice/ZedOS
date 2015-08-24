/*
 *     File Name: pmm.h
 *     Description: Page Memory Management   
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月24日 星期一 23时24分48秒
 */
#ifndef PMM_H 
#define PMM_H 

#include "multiboot.h" 

/* Thread stack size */
#define STACK_SIZE 8192 

/* Max fhysics memory */
#define PMM_MAX_SIZE 0x20000000 

/* Fhysics memory page frame size */
#define PMM_PAGE_SIZE 0x1000 

/* Fhysics memory page counts */
#define PAGE_MAX_SIZE (PMM_MAX_SIZE / PMM_PAGE_SIZE) 

/* Page mask code, align address with 4096  */
#define PHY_PAGE_MASK 0xFFFFF000 

extern uint8 kern_start[]; 
extern uint8 kern_end[]; 

/* Dynamic physic memory page counts */
extern uint32 phy_page_count; 

/* Output physics memory layout provided by BIOS */
void show_memory_map(); 

/* Initialize physics memory management */
void init_pmm(); 

/* Return an address of physics memory page */
uint32 pmm_alloc_page(); 

/* Free applyed memory */
void pmm_free_page(uint32 p); 

#endif 
