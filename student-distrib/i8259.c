/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
// These are essentialyl state tracking values for us
uint8_t master_mask; /* IRQs 0-7 (slave=2) */
uint8_t slave_mask; /* IRQs 8-15 */

/*
 * void i8259_init(void);
 * DESCRIPTION: Inits the PIC. Sets up the master and slave set up.
 * INPUT: none
 * OUTPUTS: none
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes PIC to create IRQs
*/
void
i8259_init(void)
{
    // default mask values (don't mask out slave on master, otherwise everything masked)
    slave_mask = DEFAULT_SLAVE_MASK;
    master_mask = DEFAULT_MASTER_MASK;

    // write init control word 1
    outb(ICW1, MASTER_8259_PORT);
    io_wait();
    outb(ICW1, SLAVE_8259_PORT);
    io_wait();

    // write init control word 2
    outb(ICW2_MASTER, MASTER_8259_PORT_DATA);
    io_wait();
    outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA);
    io_wait();

    // write init control word 3
    outb(ICW3_MASTER, MASTER_8259_PORT_DATA);
    io_wait();
    outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA);
    io_wait();

    // write init control word 4
    outb(ICW4, MASTER_8259_PORT_DATA);
    io_wait();
    outb(ICW4, SLAVE_8259_PORT_DATA);
    io_wait();

    // write initial masks to synchronize our state with device state
    outb(master_mask, MASTER_8259_PORT_DATA);
    io_wait();
    outb(slave_mask, SLAVE_8259_PORT_DATA);
}

/*
 * void enable_irq(uint32_t irq_num)
 * DESCRIPTION: unmasks the interrupt on a specifed line on the PIC
 * INPUT: uint32_t irq_num
 * OUTPUTS: none
 * RETURN VALUE: none.
 * SIDE EFFECTS: allows interrupts on a specific line
*/
void
enable_irq(uint32_t irq_num)
{
    uint8_t mask;
    // if IRQ line >=8, we want to use the slave - otherwise master
    uint16_t port_number = (irq_num >= NUM_INTERRUPTS_PER_PIC) ? SLAVE_8259_PORT_DATA : MASTER_8259_PORT_DATA;
    // if we're on the slave interrupt handler, subtract number of interrupts
    if(port_number == SLAVE_8259_PORT_DATA) irq_num -= NUM_INTERRUPTS_PER_PIC;

    // determine new mask
    mask = inb(port_number) & ~(1 << irq_num);

    // update saved state
    if(port_number == SLAVE_8259_PORT_DATA) {
        slave_mask = mask;
    } else {
        master_mask = mask;
    }

    // output new mask
    outb(mask, port_number);
}

/*
 * void disable_irq(uint32_t irq_num)
 * DESCRIPTION: masks the interrupt on a specifed line on the PIC
 * INPUT: uint32_t irq_num
 * OUTPUTS: none
 * RETURN VALUE: none.
 * SIDE EFFECTS: stops interrupts from being registered on a specific line
*/
void
disable_irq(uint32_t irq_num)
{
    uint8_t mask;
    // if IRQ line >=8, we want to use the slave - otherwise master
    uint16_t port_number = (irq_num >= NUM_INTERRUPTS_PER_PIC) ? SLAVE_8259_PORT_DATA : MASTER_8259_PORT_DATA;

    // subtract number of interrupts from irq num if we're talking to the slave
    if(port_number == SLAVE_8259_PORT_DATA) irq_num -= NUM_INTERRUPTS_PER_PIC;

    // determine new mask
    mask = inb(port_number) | (1 << irq_num);

    // update saved state
    if(port_number == SLAVE_8259_PORT_DATA) {
        slave_mask = mask;
    } else {
        master_mask = mask;
    }

    // output new mask
    outb(mask, port_number);
}

/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
    // if EOI is for an interrupt on the slave, we need to send an EOI to both devices. otherwise, just the master.
    if(irq_num >= NUM_INTERRUPTS_PER_PIC) {
        outb(EOI | (NUM_INTERRUPTS_PER_PIC - irq_num), SLAVE_8259_PORT); // adjusted IRQ num for slave | EOI
        outb(EOI | SLAVE_IRQ, MASTER_8259_PORT); // SLAVE_IRQ | EOI for master
    } else {
        outb(EOI | irq_num, MASTER_8259_PORT); // EOI | irq_num for master
    }
}

