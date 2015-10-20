/*
 *     File Name: initrd.h
 *     Description: Defines the interface for and structures relating to the initial ramdisk.  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.10.20 19:34:14
 */
#ifndef INITRD_H 
#define INITRD_H 

#include "../libc/types.h" 
#include "fs.h" 

typedef struct
{
	uint32 nfiles; 	/* The number of files in the ramdisk. */
} initrd_header_t; 

typedef struct
{
	uint8 magic; 	/* Magic number, for error checking. */
	int8 name[64]; 	/* Filename. */
	uint32 offset; 	/* Offset in the initrd that the filse starts. */
	uint32 length; 	/* Length of the file. */
} initrd_file_header_t; 

/* Initialises the initial ramdisk. 
 * It gets passed the address of the multiboot modules,
 * and returns a commpleted filesystem node. 
 */
fs_node_t *initialise_initrd(uint32 location); 

#endif 

