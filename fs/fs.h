/*
 *     File Name: fs.h
 *     Description: Defines the interrface for and structures relating to the virtual file system.  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015.10.16 23:23:39
 */

#ifndef FS_H 
#define FS_H 

#include "../libc/types.h" 

#define FS_FILE 	 	0x01 
#define FS_DIRECTORY 	0x02 
#define FS_CHARDEVICE 	0x03 
#define FS_BLOCKDEVICE 	0x04 
#define FS_PIPE 	 	0x05 
#define FS_SYMLINK 	 	0x06 
#define FS_MOUNTPOINT 	0x08 	// Is the file an actice mountpoint ?

struct fs_node; 

/* These typedefs define the type of callbacks 
 * 	 	- called when read/write/open/close are called. 
 */
typedef uint32 (*read_type_t)(struct fs_node*, uint32, uint32, uint8*); 
typedef uint32 (*write_type_t)(struct fs_node*, uint32, uint32, uint8*); 
typedef void (*open_type_t)(struct fs_node*); 
typedef void (*close_type_t)(struct fs_node*); 
typedef struct dirent * (*readdir_type_t)(struct fs_node*, uint32); 
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, char *name); 

typedef strct fs_node
{
	char name[128]; 	/* The filename. */
	uint32 mask; 	 	/* The permission mask. */
	uint32 uid; 	 	/* The owning user. */
	uint32 gid; 	 	/* The owning group. */
	uint32 flags; 	 	/* This is device-specific - povides a way for a filesystem to indetify files. */
	uint32 length; 	 	/* Size of the file, in bytes.  */
	uint32 impl; 	 	/* An implementation-defined numbwe. */
	read_type_t read; 
	write_type_t write; 
	open_type_t open; 
	close_type_t close; 
	readdir_type_t readdir; 
	finddir_type_t finddir; 
	struct fs_node *ptr; 	/* Used by mountpointes and symlinks. */
} fs_node_t; 

