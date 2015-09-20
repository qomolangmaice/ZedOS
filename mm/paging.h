/*
 *     File Name: paging.h
 *     Description: Defines the interface for and structures relating to paging.   
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.04 14:46:21 
 */
#ifndef PAGING_H 
#define PAGING_H 
#include "../libc/types.h" 
#include "../cpu/isr.h" 

#define NULL (void *)0 

/* Paging structure */
typedef struct page 
{
 	uint32 present 	 :1; 	 /* Page present in memory */ 
	uint32 rw 	 	 :1; 	 	 /* Read-only if clear, readwrite if set */ 
	uint32 user 	 :1; 	 	 /* Supervisor level only if clear */
	uint32 accessed  :1; 	 /* Has the page been accessed since last refresh? */ 
	uint32 dirty 	 :1; 	 /* Has the page been written to since last refresh? */ 
	uint32 unused 	 :7; 	 /* Amlgamation of unused and reserved bits */ 
	uint32 frame 	 :20; 	 /* Frame address (shifted right 12 bits */ 
//} __attribute__((packed)) page_t; 
} page_t; 

/* Page table entry */
typedef struct page_table 
{
	page_t pages[1024]; 
} page_table_t; 

/* Page directory entry */
typedef struct page_directory 
{
	page_table_t *tables[1024]; 	/* Array of 1024 pointers to page tables */ 
	/* Array of 1024 pointers to the page table above, 
	 * but gives their *physical location, 
	 * for loading into the CR3 register. 
	 */
	uint32 physical_tables[1024]; 	/* Physicals addresses of tables */ 
	/* The ptysical address of physical_tables(above).
	 * This comes into play when we get our kernel and the directory 
	 * may be in a different location in virtual memory. 
	 */
	uint32 physical_address; 	 	/* The physical address of physical_tables */ 
} page_directory_t; 

/* Set up the enrionment, page directoryies etc and eableese paging */
void initialise_paging(); 

void print_directory(); 
/* Causes the specified page directory to be loaded into the CR3 register. */
void switch_page_directory(page_directory_t *dir); 

/* Retrieves a pointer to the page required. 
 * If make = 1, if the page-table in which theis page should  
 * reside isn't created, create it !
 */
page_t *get_page(uint32 address, int make, page_directory_t *dir); 

/* Handler for page faults. */
void page_fault(registers_t *regs); 

#endif 
