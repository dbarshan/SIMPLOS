#include "system.h"

#define PIC_PRIMARY_COMMAND_REGISTER 0x20
#define PIC_PRIMARY_STATUS_REGISTER 0x20
#define PIC_PRIMARY_DATA_REGISTER 0x21
#define PIC_PRIMARY_INTERRUPT_MASK_REGISTER 0x21

#define PIC_SECONDARY_COMMAND_REGISTER 0xA0
#define PIC_SECONDARY_STATUS_REGISTER 0xA0
#define PIC_SECONDARY_DATA_REGISTER 0xA1
#define PIC_SECONDARY_INTERRUPT_MASK_REGISTER 0xA1


void init_pic()
{
 
 /* Initialize ICW1. set ICW4 bit and Initializtion bit*/
 outportb(PIC_PRIMARY_COMMAND_REGISTER, 0x11);
 outportb(PIC_SECONDARY_COMMAND_REGISTER, 0x11);

 /* Initialize ICW2. Remap hardware interrupts. */
 outportb(PIC_PRIMARY_DATA_REGISTER, 0x20);
 outportb(PIC_SECONDARY_DATA_REGISTER, 0x28);

 /* Initialize ICW3. IRQ2 from master to slave in binary format.IRQ from slave to master in decimal format */
 outportb(PIC_PRIMARY_DATA_REGISTER, 0x04);
 outportb(PIC_SECONDARY_DATA_REGISTER, 0x02);

 /* Initialize ICW4. 1 for x86 mode. */
 outportb(PIC_PRIMARY_DATA_REGISTER, 0x01);
 outportb(PIC_SECONDARY_DATA_REGISTER, 0x01);

 /* Enable all IRQs */
 outportb(PIC_PRIMARY_INTERRUPT_MASK_REGISTER, 0x0);
 outportb(PIC_SECONDARY_INTERRUPT_MASK_REGISTER, 0x0);

}
