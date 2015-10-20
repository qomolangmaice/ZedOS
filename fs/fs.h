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

typedef struct fs_node
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

struct dirent 
{
	char name[128]; 	/* Filename */
	uint32 ino; 	 	/* Inode number. Required by POSIX. */
}; 

extern fs_node_t *fs_root; 	/* The root of the filesystem. */

/* Standard read/write/open/close functions. 
 * Note that these are all suffixed with _fs to distinguish them 
 * from read/write/opem/close which deal with file descriptors,
 * not files nodes. 
 */
uint32 read_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer); 

uint32 write_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer); 

void open_fs(fs_node_t *node, uint8 read, uint8 write); 

void close_fs(fs_node_t *node); 

struct dirent *readdir_fs(fs_node_t *node, uint32 index); 

fs_node_t *finddir_fs(fs_node_t *node, char *name); 

#endif 

