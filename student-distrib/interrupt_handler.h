#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

/* 
 * Functions used by interrupt handlers to execute the core functionality 
 * of the handlers. These functions are called by the exception handlers once
 * the stack operations have been performed correctly.
 *
 * The functions below are placed in order of how they are found in the IDT
 */ 

/* called when an invalid interrupt is asked to be executed */
extern void ignore_int_sub();

/* Exceptions */
extern void exception_handler_0_sub();					
// exception 1 is reserved for intel use only
extern void exception_handler_2_sub();
extern void exception_handler_3_sub();
extern void exception_handler_4_sub();
extern void exception_handler_5_sub();
extern void exception_handler_6_sub();
extern void exception_handler_7_sub();
extern void exception_handler_8_sub();
extern void exception_handler_9_sub();
extern void exception_handler_10_sub();
extern void exception_handler_11_sub();
extern void exception_handler_12_sub();
extern void exception_handler_13_sub();
extern void exception_handler_14_sub();
// exception 15 is reserved for intel use only
extern void exception_handler_16_sub();
extern void exception_handler_17_sub();
extern void exception_handler_18_sub();
extern void exception_handler_19_sub();
// exception 20-31 are reserved for intel use only

/* Interrupts - IRQs for all devices */
extern void interrupt_handler_20_sub();

/* System Call */

#endif