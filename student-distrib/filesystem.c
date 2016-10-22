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

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
{
	return 0;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	// Check the inode bounds.
	if(inode >= bootblock->inodes)
		return -1;

	int i, j, block_num_idx, block_numbers_idx;

	// Access the first block number
	block_numbers_idx = 0;
	block_num = (inodes[inode].block_numbers)[block_numbers_idx];

	// Error-checking on block_num
	if(block_num < 0 || block_num >= bootblock->datablocks)
		return -1;

	// Return 0 if offset points at the end (or outside) of our file as there's nothing to read
	if(offset >= inodes[inode].length)
		return 0;

	// Reach the point in the file corresponding to "offset"
	i = 0; // Total number of bytes traversed
	j = 0; // Index within data block

	while(i != offset)
	{
		if(i && (i % MEMORY_BLOCK)) // If we've reached the end of this data block
		{
			block_num = (inodes[inode].block_numbers)[++block_numbers_idx]; // Move to the next data block

			// Error-checking on block_num
			if(block_num < 0 || block_num >= bootblock->datablocks)
				return -1;

			j = 0; // Reset data block index
		}

		i++;
		j++;
	}

	int overall_byte_counter;

	// At the end of this while loop, block_num stores our data block index, and j stores index within that data block.

	// Copy bytes over, one at a time
	for(i = 0, j, overall_byte_counter = offset; i < length, overall_byte_counter < inodes[inode].length; i++, j++, overall_byte_counter++)
	{
		if(j && (j % MEMORY_BLOCK)) // If we've reached the end of this data block
		{
			block_num = (inodes[inode].block_numbers)[++block_numbers_idx]; // Move to the next data block

			// Error-checking on block_num
			if(block_num < 0 || block_num >= bootblock->datablocks)
				return -1;

			j = 0; // reset data block index
		}

		// Check for EOF?

		buf[i] = (datablocks[block_num]).data[j]; // Copy over data

	}

	return i; // This is the number of bytes we read
}