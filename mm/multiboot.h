/*
 *     File Name: multiboot.h
 *     Description: 
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015:08:24 Mon 23:06:49 
 */
#ifndef MULTIBOOT_H 
#define MULTIBOOT_H 
#include "../libc/types.h" 

typedef struct multiboot_t {
	uint32 flags;  	/* Multiboot version */

	uint32 mem_lower; 
	uint32 mem_upper; 
	
	uint32 boot_device; 
	uint32 cmdline; 	 	/* Kernel command line */
	uint32 mods_count; 	  	/* boot module list */
	uint32 mods_addr; 

	/* ELF format kernel section table */
	uint32 num; 
	uint32 size; 
	uint32 addr; 
	uint32 shndx; 

	uint32 mmap_length; 
	uint32 mmap_addr; 

	uint32 dirves_length; 	 	/* The size of the first drive structure */
	uint32 drives_addr;  	 	/* The address of the first drive structure */ 	 	
	uint32 config_table; 	 	/* ROM configure table */ 
	uint32 boot_loader_name; 	
	uint32 apm_table; 	 	 	/* APM table */
	uint32 vbe_control_info; 
	uint32 vbe_mode_info; 
	uint32 vbe_mode; 
	uint32 vbe_interface_seg; 
	uint32 vbe_interface_off; 
	uint32 vbe_interface_len;  
} __attribute__((packed)) multiboot_t; 


typedef struct mmap_entry_t {
	uint32 size; 	
	uint32 base_addr_low; 
	uint32 base_addr_high; 
	uint32 length_low; 
	uint32 length_high; 
	uint32 type; 
} __attribute__((packed)) mmap_entry_t;

/*  The pointer before kernel built page table */
extern multiboot_t *mboot_ptr_tmp; 	

/* The pointer after kernel built page table */
extern multiboot_t *glb_mboot_ptr; 

#endif 
