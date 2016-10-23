/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "rtc.h"
#include "paging.h"
#include "keyboard.h"
#include "filesystem.h"

extern unsigned long* idt_jmp_table;

/* Macros. */
/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define TEST_RTC 0 // Set to 0 to take out RTC tests, 1 to keep them in

/*
 * init_idt
 * DESCRIPTION: initializes the idt for exceptions, interrupts, and system calls handlers
 * INPUT: global idt variable and the idt_jmp_table from the asm
 * OUTPUTS: none
 * RETURN VALUE: none.
 * SIDE EFFECTS: modifies the idt to have handlers
*/
void init_idt()
{
	int i;

	// gets the pointer to the idt_jmp_table
	unsigned long* idt_handler = (unsigned long*) &idt_jmp_table;

	// for all values in IDT
	for(i=0; i < NUM_VEC; i++)
	{
		// if referring to a valid exception, then init handler
		if(i < VALID_EXCEPTION_RANGE && i != RESERVED_EXCEP_1 && i != RESERVED_EXCEP_15)
		{
			SET_IDT_ENTRY(idt[i],idt_handler[i]);
			idt[i].seg_selector = KERNEL_CS;
			idt[i].reserved4 = RESERVED_4_DEFAULT;
			idt[i].reserved3 = INTERRUPT_GATE;
			idt[i].reserved2 = RESERVED_2_DEFAULT;
			idt[i].reserved1 = RESERVED_1_DEFAULT;
			idt[i].size = DEFAULT_SIZE;
			idt[i].reserved0 = RESERVED_0_DEFAULT;
			idt[i].dpl = KERNEL_LEVEL_DESCRIPTOR;
			idt[i].present = INT_PRESENT;
		}
		// if referring to a valid interrupt, then init handler
		else if(IRQ_LOW_BOUND <= i && i < IRQ_HIGH_BOUND)
		{
			SET_IDT_ENTRY(idt[i],idt_handler[i]);
			idt[i].seg_selector = KERNEL_CS;
			idt[i].reserved4 = RESERVED_4_DEFAULT;
			idt[i].reserved3 = TRAP_GATE;
			idt[i].reserved2 = RESERVED_2_DEFAULT;
			idt[i].reserved1 = RESERVED_1_DEFAULT;
			idt[i].size = DEFAULT_SIZE;
			idt[i].reserved0 = RESERVED_0_DEFAULT;
			idt[i].dpl = KERNEL_LEVEL_DESCRIPTOR;
			idt[i].present = INT_PRESENT;
		}
		// if referring to system call, then init handler
		else if(i == SYSTEM_CALL_VECTOR)
		{
			SET_IDT_ENTRY(idt[i],idt_handler[SYSTEM_CALL_HANDLER]);
			idt[i].seg_selector = KERNEL_CS;
			idt[i].reserved4 = RESERVED_4_DEFAULT;
			idt[i].reserved3 = TRAP_GATE;
			idt[i].reserved2 = RESERVED_2_DEFAULT;
			idt[i].reserved1 = RESERVED_1_DEFAULT;
			idt[i].size = DEFAULT_SIZE;
			idt[i].reserved0 = RESERVED_0_DEFAULT;
			idt[i].dpl = USER_LEVEL_DESCRIPTOR;
			idt[i].present = INT_PRESENT;
		}
		// else declare handler invalid
		else
		{
			idt[i].present = INT_NOT_PRESENT;
		}
	}
}


/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void
entry (unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mbi;

	/* Clear the screen. */
	clear();

	/* Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printf ("Invalid magic number: 0x%#x\n", (unsigned) magic);
		return;
	}

	/* initialize the IDT */
	init_idt();

	/* Set MBI to the address of the Multiboot information structure. */
	mbi = (multiboot_info_t *) addr;

	/* Print out the flags. */
	printf ("flags = 0x%#x\n", (unsigned) mbi->flags);

	/* Are mem_* valid? */
	if (CHECK_FLAG (mbi->flags, 0))
		printf ("mem_lower = %uKB, mem_upper = %uKB\n",
				(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_FLAG (mbi->flags, 1))
		printf ("boot_device = 0x%#x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_FLAG (mbi->flags, 2))
		printf ("cmdline = %s\n", (char *) mbi->cmdline);

	if (CHECK_FLAG (mbi->flags, 3)) {
		int mod_count = 0;
		int i;
		module_t* mod = (module_t*)mbi->mods_addr;
		while(mod_count < mbi->mods_count) {
			printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
			printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
			printf("First few bytes of module:\n");
			for(i = 0; i<16; i++) {
				printf("0x%x ", *((char*)(mod->mod_start+i)));
			}
			printf("\n");
			mod_count++;
			mod++;
		}
	}
	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
	{
		printf ("Both bits 4 and 5 are set.\n");
		return;
	}

	/* Is the section header table of ELF valid? */
	if (CHECK_FLAG (mbi->flags, 5))
	{
		elf_section_header_table_t *elf_sec = &(mbi->elf_sec);

		printf ("elf_sec: num = %u, size = 0x%#x,"
				" addr = 0x%#x, shndx = 0x%#x\n",
				(unsigned) elf_sec->num, (unsigned) elf_sec->size,
				(unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
	}

	/* Are mmap_* valid? */
	if (CHECK_FLAG (mbi->flags, 6))
	{
		memory_map_t *mmap;

		printf ("mmap_addr = 0x%#x, mmap_length = 0x%x\n",
				(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
		for (mmap = (memory_map_t *) mbi->mmap_addr;
				(unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
				mmap = (memory_map_t *) ((unsigned long) mmap
					+ mmap->size + sizeof (mmap->size)))
			printf (" size = 0x%x,     base_addr = 0x%#x%#x\n"
					"     type = 0x%x,  length    = 0x%#x%#x\n",
					(unsigned) mmap->size,
					(unsigned) mmap->base_addr_high,
					(unsigned) mmap->base_addr_low,
					(unsigned) mmap->type,
					(unsigned) mmap->length_high,
					(unsigned) mmap->length_low);
	}

	/* Construct an LDT entry in the GDT */
	{
		seg_desc_t the_ldt_desc;
		the_ldt_desc.granularity    = 0;
		the_ldt_desc.opsize         = 1;
		the_ldt_desc.reserved       = 0;
		the_ldt_desc.avail          = 0;
		the_ldt_desc.present        = 1;
		the_ldt_desc.dpl            = 0x0;
		the_ldt_desc.sys            = 0;
		the_ldt_desc.type           = 0x2;

		SET_LDT_PARAMS(the_ldt_desc, &ldt, ldt_size);
		ldt_desc_ptr = the_ldt_desc;
		lldt(KERNEL_LDT);
	}

	/* Construct a TSS entry in the GDT */
	{
		seg_desc_t the_tss_desc;
		the_tss_desc.granularity    = 0;
		the_tss_desc.opsize         = 0;
		the_tss_desc.reserved       = 0;
		the_tss_desc.avail          = 0;
		the_tss_desc.seg_lim_19_16  = TSS_SIZE & 0x000F0000;
		the_tss_desc.present        = 1;
		the_tss_desc.dpl            = 0x0;
		the_tss_desc.sys            = 0;
		the_tss_desc.type           = 0x9;
		the_tss_desc.seg_lim_15_00  = TSS_SIZE & 0x0000FFFF;

		SET_TSS_PARAMS(the_tss_desc, &tss, tss_size);

		tss_desc_ptr = the_tss_desc;

		tss.ldt_segment_selector = KERNEL_LDT;
		tss.ss0 = KERNEL_DS;
		tss.esp0 = 0x800000;
		ltr(KERNEL_TSS);
	}

	module_t* mod = (module_t*)mbi->mods_addr;

	uint32_t filesys_start_addr = mod->mod_start; // Filesystem starting address
	uint32_t filesys_size = mod->mod_end - mod->mod_start; // Filesystem size in bytes

	/* Init paging */
	init_paging();

	/* Init the PIC */
	i8259_init();

	/* Initialize devices, memory, filesystem, enable device interrupts on the
	 * PIC, any other initialization stuff... */
	init_rtc();
	disable_irq(RTC_LINE_NO);

	init_kbd();

	// Initialize filesystem
	init_filesystem(filesys_start_addr, filesys_size);

	clear_and_reset();

	/* Enable interrupts */
    sti();

    // welcome!
    printf_t("Welcome to Snoop-Diggity-dOS 0.2\n");

    // fs_debug();

    #if TEST_RTC
	// Tests for RTC read, write
	int i;
	int rtc_ret = write_rtc(2); // Frequency 2 Hz

	for(i = 0; i < 10; i++)
	{
		read_rtc(0, NULL, 0); // Should take 5 seconds- 10 messages should display
	}

	rtc_ret = write_rtc(10); // Should return -1 (invalid frequency)
	if(rtc_ret < 0)
		printf("Invalid frequency request!\n");

	rtc_ret = write_rtc(512);

	for(i = 0; i < 512; i++)
	{
		read_rtc(0, NULL, 0); // Should take 1 second- 512 messages should display
	}

	rtc_ret = write_rtc(11412940); // Should return -1 (invalid frequency)
	if(rtc_ret < 0)
		printf("Invalid frequency request!\n");

	rtc_ret = write_rtc(0); // Should return -1 (invalid frequency)
	if(rtc_ret < 0)
		printf("Invalid frequency request!\n");
    #endif

	uint8_t buff[KEYBOARD_BUFF_SIZE];
	int last_rtc_test = -1;
	int last_read_file = -2;

	// For CTL-2 test
	dentry_t entry;
	int32_t result;
	int32_t length_in_bytes;

	first_rtc_disable = 0;
	can_print_by_name = 1;
	can_ls = 1;
	interrupt_seen = 0;


	while(1){
		if(interrupt_seen)
		{
			clear_and_reset();
			set_cursor_location(0,0);
			interrupt_seen = 0;
		}
	    if((first_rtc_disable && testVal != TEST_FOUR))
        {
            disable_irq(RTC_LINE_NO);
			clear_and_reset();
			set_cursor_location(0,0);
            first_rtc_disable = 0;
        }
		switch(testVal) {
			case (TEST_ZERO):
				read_terminal(STDIN, buff, KEYBOARD_BUFF_SIZE);
				write_terminal(STDOUT, buff, KEYBOARD_BUFF_SIZE, STDIN);
                can_print_by_name = 1;
                can_ls = 1;
                readByIndex = -1;
				break;
			case (TEST_ONE):
                if(can_ls == 0) break;

				// directory listing
				int entry_count, i;
				char fn[FILE_NAME_BUF_SIZE];
				dentry_t* entries = get_dir_entries_array(&entry_count);
				for(i = 0; i < entry_count; i++) {
					// entries[i]->filename;
					// filetype inode
					strncpy(fn, (int8_t*) entries[i].filename, FILE_NAME_SIZE);
					fn[FILE_NAME_SIZE] = NULL_CHAR;
					printf_t(
						"file_name: %s, file_type: %u, file_size: %u \n",
						 fn,
						 entries[i].filetype,
						 get_file_length(&entries[i])
					);
				}
                can_ls = 0;
                can_print_by_name = 1;
                readByIndex = -1;
				break;
			case (TEST_TWO):
				// Read file by name
				// Assumption for now: We only have to read a single file.

                if(can_print_by_name == 0) break;


				result = read_dentry_by_name((uint8_t*) "frame0.txt", &entry); // Read directory entry

				if(result != SUCCESS)
				{
					printf_t("Failure to read, abort.\n");
					break;
				}

				length_in_bytes = get_file_length(&entry);

				if(length_in_bytes == FAILURE)
				{
					printf_t("Failure to read, abort.\n");
					break;
				}

				uint8_t mybuf[MYBUF_SIZE]; // Assume this is enough
				int32_t bytes_read = read_data(entry.inode, FILE_BEGINNING_OFFSET, mybuf, length_in_bytes); // Read data from the file

				if(bytes_read != length_in_bytes)
				{
					printf_t("Failure to read entire file, abort.\n");
					break;
				}

				write_terminal(STDOUT, mybuf, length_in_bytes, 1);
				printf_t("Filename: frame0.txt\n");
					
				can_print_by_name = 0;
                can_ls = 1;
                readByIndex = -1;
				break;
			case (TEST_THREE):
				// 
				if(readByIndex != last_read_file)
				{
					// Actually get the data and print the file
					if(readByIndex < 0) {
						readByIndex = 0;
					}
					char fn[FILE_NAME_BUF_SIZE];

					result = read_dentry_by_index(readByIndex, &entry); // Read directory entry

					if(result != SUCCESS)
					{
						printf_t("Failure to read directory entry %d, abort.\n", readByIndex);
						break;
					}

					length_in_bytes = get_file_length(&entry);

					if(length_in_bytes == FAILURE)
					{
						printf_t("Failure to get length of %d, abort.\n", readByIndex);
						break;
					}

					uint8_t mybuf[MEMORY_BLOCK]; // buffer, read 4k at a time
                                        int32_t bytes_read;
                                        
                                        // Read data from the file
					while((bytes_read = read_data(entry.inode, FILE_BEGINNING_OFFSET, mybuf, sizeof(mybuf))) != 0) {
                                            write_terminal(STDOUT, mybuf, bytes_read, 1);
                                        }
					
                                        
					printf_t("%c", NEW_LINE);
					strncpy(fn, (int8_t*) entry.filename, FILE_NAME_SIZE);
					fn[FILE_NAME_SIZE] = NULL_CHAR;
					printf_t("Filename: %s, index %d\n", fn, readByIndex);

					last_read_file = readByIndex;
				}

                can_print_by_name = 1;
                can_ls = 1;
				break;
			case (TEST_FOUR):
				// rtcTestNumber 0-15
				if(rtcTest != last_rtc_test) {
					write_rtc(rtcTest);
					last_rtc_test = rtcTest;
				}

                first_rtc_disable = 1;
                can_print_by_name = 1;
                can_ls = 1;
                readByIndex = -1;
				break;
			case (TEST_FIVE):
                can_print_by_name = 1;
                can_ls = 1;
                readByIndex = -1;
				break;
		}
	}

	/* Execute the first program (`shell') ... */

	/* Spin (nicely, so we don't chew up cycles) */
	// asm volatile(".1: hlt; jmp .1;");
}

