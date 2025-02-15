/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "devices/i8259.h"
#include "debug.h"
#include "devices/rtc.h"
#include "paging.h"
#include "devices/keyboard.h"
#include "filesystem.h"
#include "syscalls.h"
#include "scheduling.h"

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
			idt[i].reserved3 = INTERRUPT_GATE;
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
/*
 * entry
 * DESCRIPTION: Entry into the kernel. It sets up kernel, ints all drivers, and many other things
 * INPUT: unsigned long magic, unsigned long addr
 * OUTPUTS: none
 * RETURN VALUE: none.
 * SIDE EFFECTS: sets up kernel
*/
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

	init_kbd();

	// Initialize filesystem
	init_filesystem(filesys_start_addr, filesys_size);

	// Reset the video mem backing store for each terminal and reset video memory
	initialize_video_memory();

	// inits scheduling
	init_scheduling();


	/* Enable interrupts */
    sti();

    //unsigned char buf[128];

    // welcome!
    printf_t("Welcome to Snoop-Diggity-dOS 0.2\n");

    // minimal tests for read and write terminal
    // printf_t("Read Terminal Test\n");
    // int32_t read_term_bytes;
    // while((read_term_bytes = read_terminal(STDIN,buf,128))<= 0);
    // printf_t("Write Terminal Test\n");
    // write_terminal(STDOUT,buf, read_term_bytes);

	/* Execute the first program (`shell') ... */

	internal_execute((uint8_t*) "shell", FIRST_TERM_SHELL);

	/* Spin (nicely, so we don't chew up cycles) */
	asm volatile(".1: hlt; jmp .1;");
}
