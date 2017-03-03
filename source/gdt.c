#include "system.h"

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_midle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

typedef struct gdt_entry GDT;

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

typedef struct gdt_ptr GDTR;

GDT gdt[3];
GDTR gdtr;

void set_gdt(char num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
    
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_midle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void init_gdt()
{
    set_gdt(0, 0, 0, 0, 0);
    set_gdt(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    set_gdt(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    gdtr.limit = (sizeof(GDT) * 3) - 1;
    gdtr.base =  (unsigned int )gdt;
    gdt_load();

    
}
