#include "system.h"

/* Defines an IDT entry */
struct IDT_ENTRY
{
    unsigned short int base_low;
    unsigned short int selector;
    unsigned short int flags;
    unsigned short int base_high;
} __attribute__((packed));

typedef struct IDT_ENTRY IDT;

struct IDT_PTR
{
    unsigned short int limit;
    unsigned int base;
} __attribute__((packed));

typedef struct IDT_PTR IDTR;

IDT idt[256];
IDTR idtr;

void set_idt(unsigned char offset, unsigned int base, unsigned short selector, unsigned char flags)
{

    idt[offset].base_low = (base & 0xFFFF);
    idt[offset].base_high = (base >> 16) & 0xFFFF;
    idt[offset].selector = selector;
    idt[offset].flags = flags ;
    idt[offset].flags = (idt[offset].flags << 8)& 0xFF00 ;
}


void init_idt()
{
    int i;
    memset(idt, 0, sizeof(IDT) * 256);

    set_idt(0,  (unsigned int)isr0, 0x08, 0x8E);
    set_idt(1,  (unsigned int)isr1, 0x08, 0x8E);
    set_idt(2,  (unsigned int)isr2, 0x08, 0x8E);
    set_idt(3,  (unsigned int)isr3, 0x08, 0x8E);
    set_idt(4,  (unsigned int)isr4, 0x08, 0x8E);
    set_idt(5,  (unsigned int)isr5, 0x08, 0x8E);
    set_idt(6,  (unsigned int)isr6, 0x08, 0x8E);
    set_idt(7,  (unsigned int)isr7, 0x08, 0x8E);
    set_idt(8,  (unsigned int)isr8, 0x08, 0x8E);
    set_idt(9,  (unsigned int)isr9, 0x08, 0x8E);
    set_idt(10, (unsigned int)isr10, 0x08, 0x8E);
    set_idt(11, (unsigned int)isr11, 0x08, 0x8E);
    set_idt(12, (unsigned int)isr12, 0x08, 0x8E);
    set_idt(13, (unsigned int)isr13, 0x08, 0x8E);
    set_idt(14, (unsigned int)isr14, 0x08, 0x8E);
    set_idt(15, (unsigned int)isr15, 0x08, 0x8E);
    set_idt(16, (unsigned int)isr16, 0x08, 0x8E);
    set_idt(17, (unsigned int)isr17, 0x08, 0x8E);
    set_idt(18, (unsigned int)isr18, 0x08, 0x8E);
    set_idt(19, (unsigned int)isr19, 0x08, 0x8E);
    set_idt(20, (unsigned int)isr20, 0x08, 0x8E);
    set_idt(21, (unsigned int)isr21, 0x08, 0x8E);
    set_idt(22, (unsigned int)isr22, 0x08, 0x8E);
    set_idt(23, (unsigned int)isr23, 0x08, 0x8E);
    set_idt(24, (unsigned int)isr24, 0x08, 0x8E);
    set_idt(25, (unsigned int)isr25, 0x08, 0x8E);
    set_idt(26, (unsigned int)isr26, 0x08, 0x8E);
    set_idt(27, (unsigned int)isr27, 0x08, 0x8E);
    set_idt(28, (unsigned int)isr28, 0x08, 0x8E);
    set_idt(29, (unsigned int)isr29, 0x08, 0x8E);
    set_idt(30, (unsigned int)isr30, 0x08, 0x8E);
    set_idt(31, (unsigned int)isr31, 0x08, 0x8E); 

    for( i = 32; i<256; i++)
       set_idt(i, (unsigned int)default_handler, 0x08, 0x8E); 


    idtr.limit = (sizeof (IDT) * 256) - 1;
    idtr.base =  (unsigned int)idt;
    idt_load();

 }

void isr0()
{
   k_puts("\nDivide by 0 exception\n");

   while(1); 
}

void isr1()
{
  k_puts("\nDebug Exception\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr2()
{
  k_puts("\nNon Maskable interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr3()
{
  k_puts("\nBreakpoisr\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr4()
{
  k_puts("\ninto Detected Overflow\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr5()
{
  k_puts("\nOut of Bounds\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr6()
{
  k_puts("\nInvalid Opcode Exception\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr7()
{
  k_puts("\nNo Coprocessor\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr8()
{
  k_puts("\nDouble Fault\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr9()
{
  k_puts("\nCoprocessor Segment Overrun Exception\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr10()
{
  k_puts("\nBad TSS Exception\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr11()
{
  k_puts("\nSegment Not Present\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr12()
{
  k_puts("\nStack Fault\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr13()
{
  k_puts("\nGeneral Protection Fault\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr14()
{
  k_puts("\nPage Fault\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr15()
{
  k_puts("\nUnknown interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr16()
{
  k_puts("\nCoprocessor Fault\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr17()
{
  k_puts("\nAlignment Check (486+)\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr18()
{
  k_puts("\nMachine Check (Pentium/586+)\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr19()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr20()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr21()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr22()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr23()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr24()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr25()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr26()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr27()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr28()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr29()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr30()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}
void isr31()
{
  k_puts("\nReserved interrupt\n");
  k_puts("\nSystem Halted\n");
  while(1);
}

void default_handler()
{
   k_puts("\nUnspecified software interrupt.\n");

}
