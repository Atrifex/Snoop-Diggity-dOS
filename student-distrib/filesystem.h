#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"

// size of filenames in bytes
#define FILE_NAME_SIZE 32

#define THREE_BYTES 3

// and buffers need one more char since filenames might not be null-terminated (verylarge.......tx)
#define FILE_NAME_BUF_SIZE FILE_NAME_SIZE + 1

// reserved space in dir entries
#define FILE_RES_SIZE 24

// size of a block in bytes
#define FS_BLOCK_LENGTH 4096

// size of bootblock reserved, in bytes
#define BOOT_RES_SIZE 52

// max number of files
#define NUM_FILES_CAP 63

// ofset for beginning of a file
#define FILE_BEGINNING_OFFSET 0

// maximum amount of data blocks composing a file
#define MAX_BLOCKS 1023

// file type constants
#define FILETYPE_DEVICE 0
#define FILETYPE_DIRECTORY 1 
#define FILETYPE_REGULAR 2

// for code clarity.
typedef unsigned char fs_data_t;

// data block (BLOCK_LENGTH bytes)
typedef struct {
    fs_data_t data[FS_BLOCK_LENGTH];
} data_block_t;

// directory entry structure
typedef struct {
    unsigned char filename[FILE_NAME_SIZE];
    unsigned long filetype;
    unsigned long inode;
    unsigned char reserved[FILE_RES_SIZE];
} dentry_t;

// FS boot block structure
typedef struct {
    unsigned long direntries;
    unsigned long inodes;
    unsigned long datablocks;
    unsigned char reserved[BOOT_RES_SIZE];
    dentry_t      files[NUM_FILES_CAP]; // up to 63 can either declare as static or dynamic
} boot_block_t;

// inode structure
typedef struct {
    uint32_t length;
    uint32_t block_numbers[MAX_BLOCKS];
} inode_t;

// Initializes our kernel's internal structure for the filesystem
extern void init_filesystem(uint32_t start_addr, uint32_t size);

// Reads a directory entry given the name of a file
extern int32_t read_dentry_by_name(const uint8_t * fname, dentry_t * dentry);

// Reads a directory given an index node number
extern int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry);

// Reads data from a file given the inode and the offset into the file's data
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

// Returns the length of a file in bytes
extern int32_t get_file_length(dentry_t * entry);

extern int32_t read_file(int32_t fd, void* buf, int32_t nbytes);

extern int32_t read_directory(int32_t fd, void* buf, int32_t nbytes);

extern int32_t write_file(int32_t fd, const void* buf, int32_t nbytes);

extern int32_t close_file(int32_t fd);

extern int32_t open_file (const uint8_t* filename);

extern int32_t write_directory(int32_t fd, const void* buf, int32_t nbytes);

extern int32_t close_directory(int32_t fd);

extern int32_t open_directory (const uint8_t* filename);

extern inode_t* get_inode_ptr(uint32_t inode_idx);

extern uint32_t get_inode_num(inode_t* inode_ptr);


#endif /* ifndef FILESYSTEM_H */
