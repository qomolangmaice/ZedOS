/*
 *     File Name: pmm.c
 *     Description: Page Memory Management  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:24 Mon 23:41:33 
 */
#include "multiboot.h" 
#include "pmm.h" 
#include "../drivers/screen.h" 
#include "../libc/string.h" 
#include "../libc/printf.h" 


/* Fhysics memory page management stack */
static uint32 pmm_stack[PAGE_MAX_SIZE + 1]; 

/* Fhysics memory management stack pointer */
static uint32 pmm_stack_top; 

/* Fhysics memory page counts */
uint32 phy_page_count; 

void show_memory_map()
{
	uint32 mmap_addr = glb_mboot_ptr->mmap_addr; 
	uint32 mmap_length = glb_mboot_ptr->mmap_length; 

	print("Memory map: \n"); 

	mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr; 
	for (mmap = (mmap_entry_t *)mmap_addr; (uint32)mmap < mmap_addr + mmap_length; mmap++) 
	{
		printf("base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n", 
				(uint32)mmap->base_addr_high, 
				(uint32)mmap->base_addr_low, 
				(uint32)mmap->length_high, 
				(uint32)mmap->length_low, 
				(uint32)mmap->type); 
	}
}

void init_pmm()
{
	mmap_entry_t *mmap_start_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr; 
	mmap_entry_t *mmap_end_addr = (mmap_entry_t *)glb_mboot_ptr->mmap_addr + glb_mboot_ptr->mmap_length; 
 	
	mmap_entry_t *map_entry; 

	for (map_entry = mmap_start_addr; map_entry < mmap_end_addr; map_entry++) 
	{
		if(map_entry->type == 1 && map_entry->base_addr_low == 0x100000)
		{
			uint32 page_addr = map_entry->base_addr_low + (uint32)(kern_end - kern_start); 
			uint32 length = map_entry->base_addr_low + map_entry->length_low; 

			while (page_addr < length && page_addr <= PMM_MAX_SIZE) 
			{
				//pmm_free_page(page_addr); 
				page_addr += PMM_PAGE_SIZE; 
				phy_page_count++; 
			}
		}
	}
}

/*
uint32 pmm_alloc_page()
{
	assert(pmm_stack_top != 0, "out of memory");

	uint32 page = pmm_stack[pmm_stack_top--]; 

	return page; 
}

void pmm_free_page(uint32 p)
{
	assert(pmm_stack_top != PAGE_MAX_SIZE, "out of pmm_stack stack"); 

	pmm_stack[++pmm_stack_top] = p; 
}
*/ 
