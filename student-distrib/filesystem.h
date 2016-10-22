#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"

#define FILE_NAME_SIZE 32
#define FILE_NAME_LOC 0
#define FILE_TYPE_SIZE 4
#define FILE_TYPE_LOC 32
#define FILE_INODE_SIZE 4
#define FILE_INODE_LOC 36
#define FILE_RES_SIZE 24
#define FILE_RES_LOC 40
#define BYTE 8
#define FILE_TYPE_MASK
#define MEMORY_BLOCK 4000
#define DIR_ENTRY 64
#define BOOT_BLOCK_INDEX 0
#define DIR_ENT_SIZE 4
#define NUM_INODE_SIZE 4
#define NUM_INODE_LOC 4
#define DATA_BLOCK_SIZE 4
#define DATA_BLOCK_LOC 8
#define BOOT_RES_LOC 12
#define BOOT_RES_SIZE 52
#define FILETYPE_SIZE 4
#define NUM_FILES_CAP 62

typedef struct {
 unsigned char filename[FILE_NAME_SIZE];
 unsigned long filetype;
 unsigned long inode;
 unsigned char reserved[FILE_RES_SIZE];
} dentry_t;

typedef struct {
 unsigned long direntries;
 unsigned long inodes;
 unsigned long datablocks;
 unsigned char reserved[BOOT_RES_SIZE];
} boot_block_t;

typedef struct {
 boot_block_t boot;
 dentry_t self;
 dentry_t files[NUM_FILES_CAP]; //up to 62 can either declare as static or dynamic
} file_system_t;

// Initializes our kernel's internal structure for the filesystem
extern void init_filesystem(int start_addr, int size);

// Reads a directory entry given the name of a file
extern int32_t read_dentry_by_name(const uint8* fname, dentry_t* dentry);

// Reads a directory given an index node number
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

// Reads data from a file given the inode and the offset into the file's data
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
