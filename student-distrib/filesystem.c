#include "filesystem.h"

// pointer to boot block, first inode (inode array), and first data block
boot_block_t* bootblock;
inode_t* inodes;
data_block_t* datablocks;

/*
 * void init_filesystem
 * DESCRIPTION: Initializes our kernel's structure for the filesystem
 * INPUTS   : start_addr (starting address of the filesystem), size (size in bytes of the filesystem image)
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Constructs structure for the filesystem.
*/
void init_filesystem(uint32_t start_addr, uint32_t size)
{
	
}

int32_t read_dentry_by_name(const uint8* fname, dentry_t* dentry)
{
	return 0;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	return 0;
}