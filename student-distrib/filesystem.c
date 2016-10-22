#include "filesystem.h"
#include "lib.h"

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
	// start_addr points to our boot block
	bootblock = (boot_block_t*) start_addr;

	// first inode will be start_addr + block_size
	inodes = (inode_t*) ( start_addr + MEMORY_BLOCK );

	// first data block will be start_addr + memory_block + (block_size*number of inodes)
	datablocks = (data_block_t*) ( start_addr + MEMORY_BLOCK + MEMORY_BLOCK*bootblock->inodes);
}

void fs_debug() {
	printf(
		"There are %d dir entries, %d inodes and %d data blocks\n",
		 bootblock->direntries, bootblock->inodes, bootblock->datablocks
	);

	int i = 0;
	for(i = 0; i < bootblock->direntries; i++) {
		printf("%s\n", bootblock->files[i].filename);
	}
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
	int i;	// declare iterator
	for(i = 0; i < NUM_FILES_CAP; i++)
	{
		if(strncmp((int8_t*) fname, (int8_t*) bootblock->files[i].filename, MAXBITS_TO_READ)) {
			memcpy((void*) dentry, (const void*) &bootblock->files[i], sizeof(dentry_t));
			return SUCCESS;
		}
	}
	return FAILURE;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	if(index >= bootblock->direntries)
		return FAILURE; 
	
	memcpy((void*) dentry, (const void*) &bootblock->files[index], sizeof(dentry_t));
	return SUCCESS;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	return SUCCESS;
}
