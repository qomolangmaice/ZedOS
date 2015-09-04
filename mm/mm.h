/*
 *     File Name: mm.h
 *     Description: Memory Management  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.04 14:46:21 
 */
#ifndef MM_H 
#define MM_H 
#include "../libc/types.h" 
#include "../cpu/isr.h" 
#include "../cpu/irq.h" 

#define NULL (void *)0 

/* Paging structure */
typedef struct page 
{
 	uint32 present:1; 	 /* Page present in meory */ 
	uint32 rw:1; 	 	 /* Read-only if clear, readwrite if set */ 
	uint32 user:1; 	 	 /* Supervisor level only if clear */
	uint32 accessed:1; 	 /* Has the page been accessed since lat refresh? */ 
	uint32 dirty:1; 	 /* Has the page been written to since last refresh? */ 
	uint32 unused:7; 	 /* Amlgamation of unused and reserved bits */ 
	uint32 frame:20; 	 /* Frame address (shifted right 12 bits */ 
} __attribute__((packed)) page_t; 

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

page_directory_t *kernel_directory; 
page_directory_t *current_directory; 

/* Memory Management */
uint32 placement_pointer; 
uint32 placement_address; 
void kmalloc_startat(uint32 address); 
uint32 kmalloc_real(size_t size, int align, uint32 *phys); 
uint32 kmalloc(size_t size); 
uint32 kvmalloc(size_t size); 
uint32 kmalloc_p(size_t size, uint32 *phys); 
uint32 kvmalloc_p(size_t size, uint32 *phys); 

/* Paging */
//void paging_install(uint32 memsize); 
void paging_install(); 
void switch_page_directory(page_directory_t *new); 
page_t *get_page(uint32 address, int make, page_directory_t *dir); 
void page_fault(registers_t *regs); 
void dma_frame(page_t *page, int, int, uint32); 
//void debug_print_directory(); 

/*
// Heap install 
void heap_install(); 

void alloc_frame(page_t *page, int int is_kernel, int is_writeable); 
void free_frame(page_t *page); 

uint32 memory_use(); 
uint32 memory_total(); 

// klmalloc 
void * __attribute__ ((malloc)) malloc(size_t size); 
void * __attribute__ ((malloc)) realloc(void *ptr, size_t size); 
void * __attribute__ ((malloc)) calloc(size_t nmemb, size_t size); 
void * __attribute__ ((malloc)) valloc(size_t size);  
void free(void *ptr);   

*/ 
#endif 
