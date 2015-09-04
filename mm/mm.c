/*
 *     File Name: mm.c
 *     Description: Kernel Memory Management  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.09.04 Fri 15:17:26
 */
#include "mm.h" 
#include "../libc/types.h" 
#include "../cpu/irq.h" 
#include "../drivers/screen.h" 
#include "../libc/printf.h" 

extern void *end; 
uint32 placement_pointer = (uint32)&end; 
//uint32 heap_end = (uint32)NULL; 

void kmalloc_startat(uint32 address) 
{
	placement_pointer = address; 
}

/* kmalloc is the kernel's dumb placement allocator */
uint32 kmalloc_real(size_t size, int align, uint32 *phys) 
{
	/*
	if(heap_end)
	{
		void *address; 
		if(align)
		{
			address = valloc(size); 
	 	}
		else 
		{
			address = malloc(size); 
		}

		if(phys)
		{
			page_t *page = get_page((uint32)address, 0, kernel_direcotry); 
			*phys = page->frame * 0x1000 + ((uint32)address & 0xFFF); 
		}
		return (uint32)address; 
	}
	*/ 

	/* If the address is not already page-aligned */
	if(align && (placement_pointer & 0xFFFF000)) 
	{
		/* Align it */
		placement_pointer &= 0xFFFF000; 
		placement_pointer += 0x1000; 	 
	}
	if(phys)
	{
		*phys = placement_pointer; 
	}

	uint32 address = placement_pointer; 
	placement_pointer += size; 

	return (uint32)address; 
}

/* Normal */
uint32 kmalloc(size_t size)
{
	return kmalloc_real(size, 0, NULL); 
}

/* Aligned */
uint32 kvmalloc(size_t size)
{
	return kmalloc_real(size, 1, NULL); 
}

/* With a physical address */
uint32 kmalloc_p(size_t size, uint32 *phys)
{
	return kmalloc_real(size, 0, phys); 
}

/* Aligned, with a physical address */
uint32 kvmalloc_p(size_t size, uint32 *phys) 
{
	return kmalloc_real(size, 1, phys); 
}

/************************ Frame Allocation *************************/
/* A bitset of frames - used or free */
uint32 *frames; 
uint32 nframes; 

/* Macros used i the bitset algorithms */
#define INDEX_FROM_BIT(b) (b / 0x20) 
#define OFFSET_FROM_BIT(b) (b % 0x20) 

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
	
	for(i = 0; i < INDEX_FROM_BIT(nframes); ++i) 
	{
		if(frames[i] != 0xFFFFFFFF) 	/* Nothing free, exit early */ 
		{
			/* at least one bit is free here */
			for(j = 0; j < 32; ++j) 
			{
				uint32 testFrame = 0x1 << j; 
				if(!(frames[i] & testFrame)) 
				{
					return i * 0x20 + j; 
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
		page->rw = (is_writeable == 1) ? 1 : 0; 
		page->user = (is_kernel == 1) ? 1 : 0; 
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
			print("No free frames!"); 
		}
		set_frame(index * 0x1000); 	/* This frame is now ours */ 
		page->present = 1; 	 	 	/* Make it as present */ 
		page->rw = (is_writeable == 1) ? 1 : 0; /* Should the page be wriatable? */ 
		page->user = (is_kernel == 1) ? 1: 0; 	/* Should th page be user-mode? */ 
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
		clear_frame(frame * 0x1000); 	/* Frame is now free again */
		page->frame = 0x0;  	/* Page now doesn't have a frame */
	}
}

/*
void dma_frame(page_t *page, int is_kernel, int is_writeable) 
{
}

uint32 memory_use() 
{
}

uint32 memory_total()  
{
}
*/ 

//void paging_install(uint32 memsize) 
void paging_install() 
{
	/* The size of physical memory. For moment we assume it is 16MB */
 	uint32 mem_end_page = 0x1000000; 

	nframes = mem_end_page / 0x1000; 
	frames = (uint32 *)kmalloc(INDEX_FROM_BIT(nframes)); 
	memory_set(frames, 0, INDEX_FROM_BIT(nframes)); 

	/* Let's make a page directory */
	kernel_directory = (page_directory_t *)kvmalloc(sizeof(page_directory_t)); 
	memory_set(kernel_directory, 0, sizeof(page_directory_t)); 
	current_directory = kernel_directory; 

	int i = 0; 
	while(i < placement_address) 
	{
		/* Kernel code is readable but not writebable from userspace */
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0); 
		i += 0x1000; 
	}

	/* Before we enable paging, we must register our page fault handler */
	register_interrupt_handler(14, page_fault); 

	/* Now enable paging */
	switch_page_directory(kernel_directory); 
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir; 
	asm volatile("mov %0, %%cr3":: "r"(&dir->physical_address)); 
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
		dir->tables[table_index] = (page_table_t *)kvmalloc_p(sizeof(page_table_t), (uint32 *)(&temp)); 
		memory_set(dir->tables[table_index], 0, sizeof(page_table_t)); 
		dir->physical_tables[table_index] = temp | 0x7; 	/* Present, R/W, user */
		return &dir->tables[table_index]->pages[address % 1024]; 
	}
	else 
	{
		return 0; 
	}
}

void page_fault(registers_t *regs) 
{
	/* A page fault has occurred */
	/* The faulting address is stored in the CR2 register */
	uint32 faulting_address; 
	asm volatile("mov %%cr2, %0":  "=r" (faulting_address)); 

	/* The error code gives us details of what happened */
	int present = !(regs->err_code & 0x1); 	/* Page not present */ 
	int rw = regs->err_code & 0x2; 	 	 	/* Write operation ? */ 
	int user = regs->err_code & 0x4; 	 	 	/* Processor wae in user-mode */ 
	int reserved = regs->err_code & 0x8; 	/* Overwriteen CPU-reserved bits of page entry */ 
	int id = regs->err_code & 0x10;  	 	/* Casued by an instruction fetch */

	/* Output an error message */
	printf("Segmentation fault.(p: %d, rw: %d, user:%d, reserved: %d, id: %d)", present, rw, user, reserved);  
 	print("\nPage fault"); 
}
