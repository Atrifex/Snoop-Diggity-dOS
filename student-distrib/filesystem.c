#include "filesystem.h"
#include "lib.h"
#include "devices/rtc.h"
#include "syscalls.h"

// pointer to boot block, first inode (inode array), and first data block
boot_block_t * bootblock;
inode_t * inodes;
data_block_t * datablocks;

static int32_t ls_count = 0; // Index of the filename to be read when we read from the directory

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
    bootblock = (boot_block_t *) start_addr;

    // first inode will be start_addr + block_size
    inodes = (inode_t *) ( start_addr + FS_BLOCK_LENGTH );

    // first data block will be start_addr + memory_block + (block_size*number of inodes)
    datablocks = (data_block_t *) ( start_addr + FS_BLOCK_LENGTH + FS_BLOCK_LENGTH * bootblock->inodes);
}

/*
 * int32_t open_file(const uint8_t* filename)
 * DESCRIPTION: Opens a file (not directory, terminal or RTC)
 * INPUTS   : filename (name of the file we're opening)
 * OUTPUTS  : none
 * RETURN VALUE: Returns 0 if we get to this function at all
 * SIDE EFFECTS: None
 */
int32_t open_file (const uint8_t* filename)
{
   // We don't need to do anything specific in this function as system call open() will handle everything already

    return SUCCESS;
}

/*
# int32_t close_file()
# DESCRIPTION: Closes a file
# INPUTS   : fd (file descriptor)
# OUTPUTS  : none
# RETURN VALUE: Returns 0 if we get to this function at all
# SIDE EFFECTS: none
*/
int32_t close_file(int32_t fd)
{
    // We don't need to do anything specific in this function as system call close() will handle everything already

    return SUCCESS;
}

/*
# int32_t write_file()
# DESCRIPTION: Write to file... or not
# INPUTS   : Ignored
# OUTPUTS  : none
# RETURN VALUE: Returns 0
# SIDE EFFECTS: none
*/
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes)
{
    return 0; // Read-only filesystem
}

/*
# int32_t read_file(int32_t fd, void* buf, int32_t nbytes)
# DESCRIPTION: Reads data from a file (not RTC, terminal, or the directory)
# INPUTS   : fd: file descriptor of the file to read, buf: buffer into which to read data, nbytes: Number of bytes to read.
# OUTPUTS  : none
# RETURN VALUE: Returns 0
# SIDE EFFECTS: none
*/
int32_t read_file(int32_t fd, void* buf, int32_t nbytes)
{
    

    // Call read_data

    return 0;
}

/*
# int32_t read_directory()
# DESCRIPTION: Read a filename from the directory
# INPUTS   : buf: buffer into which to read a filename
# OUTPUTS  : none
# RETURN VALUE: Returns 0
# SIDE EFFECTS: none
*/
int32_t read_directory(int32_t fd, void* buf, int32_t nbytes)
{
    int entry_count;
    dentry_t* entries;

    entry_count = bootblock->direntries;
    entries = bootblock->files;

    if(ls_count >= entry_count)
    {
        ls_count = 0; // So subsequent ls calls work
        return 0; // No bytes read (we read all directory entries already)
    }

    strncpy((int8_t*)buf, (int8_t*) entries[ls_count].filename, nbytes); // Copy filename (or at least, the number of bytes specified)

    ls_count++; // We just read one more file

    if(((int8_t*)buf)[31] != '\0')
        return 32; // Return the number of bytes read

    return strlen((int8_t*)buf);
}

/*
 * int32_t read_dentry_by_name
 * DESCRIPTION: Reads directory entry by filename
 * INPUTS   : fname - file name
 * OUTPUTS  : dentry - the found directory entry
 * RETURN VALUE: SUCCESS or FAILURE
 * SIDE EFFECTS: N/A.
 */
int32_t read_dentry_by_name(const uint8_t * fname, dentry_t * dentry)
{
    int i;

    for (i = 0; i < NUM_FILES_CAP; i++) { // iterate the directory entry array
        // compare the given filename against the current entry file name
        if (strncmp((int8_t *) fname, (int8_t *) bootblock->files[i].filename, FILE_NAME_SIZE) == 0) {
            // if it matches, copy the directory entry and then return
            memcpy((void *) dentry, (const void *) &bootblock->files[i], sizeof(dentry_t));
            return SUCCESS;
        }
    }
    // return failure if we didn't find anything
    return FAILURE;
}

/*
 * int32_t read_dentry_by_index
 * DESCRIPTION: Reads directory entry by index
 * INPUTS   : index - file index in directory
 * OUTPUTS  : dentry - the found directory entry
 * RETURN VALUE: SUCCESS or FAILURE
 * SIDE EFFECTS: N/A.
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t * dentry)
{
    // if the index is invalid, return failure
    if (index >= bootblock->direntries)
        return FAILURE;

    // copy and return
    memcpy((void *) dentry, (const void *) &bootblock->files[index], sizeof(dentry_t));
    return SUCCESS;
}

/*
 * int32_t get_file_length
 * DESCRIPTION: Returns the length of a file given its directory entry
 * INPUTS   : entry - directory entry ptr for a file
 * OUTPUTS  : None
 * RETURN VALUE: The length of the file in bytes if successsful, FAILURE if unsuccessful
 * SIDE EFFECTS: N/A.
 */
int32_t get_file_length(dentry_t * entry)
{
    if (entry->inode < 0 || entry->inode >= bootblock->inodes) // Input checking
        return FAILURE;

    return inodes[entry->inode].length;
}

/*
 * int32_t read_data
 * DESCRIPTION: Reads data from file pointed to by given inode
 * INPUTS   : inode - inode to read from, offset - offset in file to read, length - max amount bytes to read
 * OUTPUTS  : buf - file data
 * RETURN VALUE: number of bytes read into buffer
 * SIDE EFFECTS: N/A.
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length)
{
    // Check the inode bounds.
    if (inode >= bootblock->inodes)
        return FAILURE;

    int i, j, k, block_num, block_numbers_idx, overall_byte_counter;

    // Access the first block number
    block_numbers_idx = 0;
    block_num         = (inodes[inode].block_numbers)[block_numbers_idx];

    // Error-checking on block_num
    if (block_num < 0 || block_num >= bootblock->datablocks)
        return FAILURE;

    // Return 0 if offset points at the end (or outside) of our file as there's nothing to read
    if (offset >= inodes[inode].length)
        return 0;

    // Reach the point in the file corresponding to "offset"
    i = 0; // Total number of bytes traversed
    j = 0; // Index within data block

    while (i != offset) {
        if (i && (i % FS_BLOCK_LENGTH == 0)) {                                 // If we've reached the end of this data block
            block_num = (inodes[inode].block_numbers)[++block_numbers_idx]; // Move to the next data block

            // Error-checking on block_num
            if (block_num < 0 || block_num >= bootblock->datablocks)
                return FAILURE;

            j = 0; // Reset data block index
        }

        i++;
        j++;
    }


    // At the end of this while loop, block_num stores our data block index, and j stores index within that data block.
    // Loop/indexing variables
    i = 0;
    k = j;
    overall_byte_counter = offset;

    // Copy bytes over, one at a time
    while ((i < length) && (overall_byte_counter < inodes[inode].length)) {
        if (k && (k % FS_BLOCK_LENGTH == 0)) {                                 // If we've reached the end of this data block
            block_num = (inodes[inode].block_numbers)[++block_numbers_idx]; // Move to the next data block

            // Error-checking on block_num
            if (block_num < 0 || block_num >= bootblock->datablocks)
                return FAILURE;

            k = 0; // reset data block index
        }

        // Check for EOF?
        buf[i] = (datablocks[block_num]).data[k]; // Copy over data

        // Increment loop/indexing variables
        i++;
        k++;
        overall_byte_counter++;
    }

    return i; // This is the number of bytes we read
} /* read_data */

/*
 * inode_t* get_inode_ptr
 * DESCRIPTION: Returns a pointer to the inode specified by inode_idx
 * INPUTS   : inode_idx: index of the inode
 * OUTPUTS  : None
 * RETURN VALUE: Pointer to the inode corresponding to inode_idx
 * SIDE EFFECTS: N/A.
 */
inode_t* get_inode_ptr(uint32_t inode_idx)
{
    if(inode_idx < NUM_FILES_CAP)
        return &(inodes[inode_idx]);

    else
        return NULL;
}
