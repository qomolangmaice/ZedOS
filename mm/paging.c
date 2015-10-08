/*
 *     File Name: paging.c
 *     Description: Defines the interface for and structures relating to paging.  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.04 Fri 15:17:26
 */
#include "paging.h" 
#include "kheap.h" 
#include "../libc/types.h" 
#include "../cpu/irq.h" 
//#include "../drivers/screen.h" 
#include "../libc/printf.h" 
#include "../libc/system.h" 

/* The kernel's page directory */
page_directory_t *kernel_directory = 0; 

/* The kernel's page directory */
page_directory_t *current_directory = 0; 

/* A bitset of frames - used or free. */
uint32 *frames; 
uint32 nframes; 

/* Defined in kheap.c */
extern uint32 placement_address; 
extern heap_t *kheap; 

/* Macros used in the bitset algorithms.  */
#define INDEX_FROM_BIT(a) 	(a / (8 * 4)) 
#define OFFSET_FROM_BIT(a) 	(a % (8 * 4)) 

/* Static function to clear a bit in the frames bitset */
static void set_frame(uint32 frame_addr) 
{
	uint32 frame = frame_addr / 0x1000; 
	uint32 index = INDEX_FROM_BIT(frame); 
	uint32 offset = OFFSET_FROM_BIT(frame); 
	frames[index] |= (0x1 << offset); 
}

/* Static function to clear a bit is in the frames bitset */
static void clear_frame(uint32 frame_addr) 
{
	uint32 frame = frame_addr / 0x1000; 
	uint32 index = INDEX_FROM_BIT(frame); 
	uint32 offset = OFFSET_FROM_BIT(frame); 
	frames[index] &= ~(0x1 << offset); 
}

/* Static function to test if a bit is set */
static uint32 test_frame(uint32 frame_addr) 
{
	uint32 frame = frame_addr / 0x1000; 
	uint32 index = INDEX_FROM_BIT(frame); 
	uint32 offset = OFFSET_FROM_BIT(frame); 
	return (frames[index] & (0x1 << offset)); 
}

/* Static function to find the first free frame */
static uint32 first_frame()
{
	uint32 i, j; 
	
	for(i = 0; i < INDEX_FROM_BIT(nframes); i++) 
	{
		if(frames[i] != 0xFFFFFFFF) 	/* Nothing free, exit early */ 
		{
			/* at least one bit is free here */
			for(j = 0; j < 32; j++) 
			{
				uint32 testFrame = 0x1 << j; 
				if(!(frames[i] & testFrame)) 
				{
					return i*4*8 + j; 
				}
			}
		}
	}
}

/* Function to allocate a frame */
void alloc_frame(page_t *page, int is_kernel, int is_writeable) 
{
	if(page->frame != 0)
	{
		//page->rw = (is_writeable == 1) ? 1 : 0; 
		//page->user = (is_kernel == 1) ? 1 : 0; 
		return;
	}
	else
	{
		/* index is now the index of the first free frame */
		uint32 index = first_frame(); 
		if(index == (uint32)-1)
		{
			/* PANIC is just a macro that prints a message to 
			 * the screen then hits an infinite loop
			 */
			printf("No free frames!"); 
		}
		set_frame(index * 0x1000); 	/* This frame is now ours */ 
		page->present = 1; 	 	 	/* Make it as present */ 
		page->rw = (is_writeable) ? 1 : 0; /* Should the page be wriatable? */ 
		page->user = (is_kernel) ? 0 : 1; 	/* Should th page be user-mode? */ 
		page->frame = index; 
	}
}

/* Function to deallocate a frame */
void free_frame(page_t *page) 
{
	uint32 frame; 

	if(!(frame = page->frame)) 
	{
		return; 	
	}
	else
	{
		//clear_frame(frame * 0x1000); 	/* Frame is now free again */
		clear_frame(frame); 	/* Frame is now free again */
		page->frame = 0x0;  	/* Page now doesn't have a frame */
	}
}

unsigned short memory_use() 
{
	unsigned short ret = 0; 
	uint32 i, j; 
	for (i = 0; i < INDEX_FROM_BIT(nframes); ++i) 
	{
		for (j = 0; j < 32; ++j) 
	 	{
			uint32 testFrame = 0x1 << j; 
			if (frames[i] & testFrame) 
			{
				ret++; 
			}
		}
	}
	return ret * 4; 
}

unsigned short memory_total() 
{
	return nframes * 4; 
}

void initialise_paging() 
{
	printf("\nSetup paging...\n"); 
	/* The size of physical memory. For moment we assume it is 16MB */
 	uint32 mem_end_page = 0x1000000; 

	nframes = mem_end_page / 0x1000; 
	frames = (uint32 *)kmalloc(INDEX_FROM_BIT(nframes)); 
	memory_set(frames, 0, INDEX_FROM_BIT(nframes)); 

	/* Let's make a page directory */
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t)); 
	memory_set(kernel_directory, 0, sizeof(page_directory_t)); 
	current_directory = kernel_directory; 

	/* Map some pages in the kernel heap area. 
	 * Here we call get_page but not alloc_frames yet because
	 * they need to be indentity mapped first below, and yet we can't increase 
	 * placement_address between indentity mapping and enabling the heap! */
	int i = 0; 
	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
		get_page(i, 1, kernel_directory); 

	/* We need to indentity map (phys addr = virt addr) from 
	 * 0x0 to the end of used memory, so we can access this 
	 * transparently, as if paging wasn't enabled. 
	 * NOTE that we use a while loop here enabled deliberately. 
	 * inside the loop body we actually change placement_address 
	 * by calling kmalloc(). A while loop causes this to be 
	 * computed on-the-fly rather than once at the start. 
	 * Allocate a little bit extra so the kernel heap can be 
	 * inititialise properly. */
	i = 0; 
	while(i < placement_address + 0x1000) 
	{
		/* Kernel code is readable but not writebable from userspace */
		alloc_frame( get_page(i, 1, kernel_directory), 0, 0); 
		i += 0x1000; 
	}

	/* Now allocate those pages we mapped earlier. */
	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000) 
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0); 
	
	/* Before we enable paging, we must register our page fault handler */
	register_interrupt_handler(14, page_fault); 

	/* Now enable paging */
	switch_page_directory(kernel_directory); 

	/* Initialise the kernel page. */
	kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0); 

	//printf("\nMemory use: %d\n", memory_use()); 
	//printf("\nMemory total: %d\n", memory_total()); 
}

void print_directory() 
{
	asm volatile("cli"); 
	printf(" ---- [k:0x%x u:0x%x]\n", kernel_directory, current_directory); 
	uint32 i = 0; 
	for (i = 0; i < 1024; ++i) 
	{
		if (!current_directory->tables[i] || (uint32)current_directory->tables[i] == (uint32)0xFFFFFFFF) 
		{
			continue; 
		}
		if (kernel_directory->tables[i] == current_directory->tables[i]) 
		{
			printf("  0x%x - kern [0x%x] %d\n", current_directory->tables[i], &current_directory->tables[i], i); 
		}
		else 
		{
			printf("  0x%x - user [0x%x] %d\n", current_directory->tables[i], &current_directory->tables[i], i); 
		}
	}
	printf(" ---- [done]\n"); 
	asm volatile("sti"); 
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir; 
	asm volatile("mov %0, %%cr3":: "r"(&dir->physical_tables)); 
	uint32 cr0; 
	asm volatile("mov %%cr0, %0": "=r"(cr0)); 
	cr0 |= 0x80000000; 	/* Enable paging */ 
	asm volatile("mov %0, %%cr0":: "r"(cr0)); 

	/*
	 * mov eax, physical_address 
	 * mov cr3, eax 
	 * mov eax, cr0 
	 * or eax, 0x80000000 
	 * mov cr0, eax 
	 * jmp short .ret
	 *
	 * .ret: 
	 *  	nop 
	 *  	ret 
	 */
}

page_t *get_page(uint32 address, int make, page_directory_t *dir) 
{
	/* Turn the address into an index */
	address /= 0x1000; 
	/* Find the page table containing this address */
	uint32 table_index = address / 1024; 

	if(dir->tables[table_index]) 	/* If this table is already assigned */ 
	{
		return &dir->tables[table_index]->pages[address % 1024]; 
	}
	else if(make) 
	{
		uint32 temp; 
		dir->tables[table_index] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &temp); 
		dir->physical_tables[table_index] = temp | 0x7; 	/* Present, R/W, user */
		return &dir->tables[table_index]->pages[address % 1024]; 
	}
	else 
	{
		return 0; 
	}
}

void page_fault(registers_t regs) 
{
	/* A page fault has occurred */
	/* The faulting address is stored in the CR2 register */
	uint32 faulting_address; 
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address)); 

	/* The error code gives us details of what happened */
	int present = !(regs.err_code & 0x1); 	/* Page not present */ 
	int rw = regs.err_code & 0x2; 	 	 	/* Write operation ? */ 
	int user = regs.err_code & 0x4;  	 	/* Processor was in user-mode? */ 
	int reserved = regs.err_code & 0x8; 	/* Overwriteen CPU-reserved bits of page entry */ 
	int id = regs.err_code & 0x10;  	 	/* Casued by an instruction fetch */

 	//print("\nPage fault!\n"); 
	/* Output an error message */
	printf("\nPage fault: \n");  
	if (present) 
		printf(" 	present\n"); 
	if (rw) 
		printf(" 	read-only\n"); 
	if (user) 
		printf(" 	user-mode\n"); 
	if (reserved) 
		printf(" 	reserved\n"); 
	printf(" 	Faulting address: "); 
	print_hex(faulting_address); 
	printf("\n"); 
	PANIC("Page fault"); 
}
