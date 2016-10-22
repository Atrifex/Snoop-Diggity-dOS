#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"

// Initializes our kernel's internal structure for the filesystem
extern void init_filesystem(int start_addr, int size);

// Reads a directory entry given the name of a file
extern int32_t read_dentry_by_name(const uint8* fname, dentry_t* dentry);

// Reads a directory given an index node number
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

// Reads data from a file given the inode and the offset into the file's data
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
