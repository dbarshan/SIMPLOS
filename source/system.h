#ifndef __SYSTEM_H
#define __SYSTEM_H


/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

/* system.c */
extern void *memcpy(void *dest, const void *src, int count);
extern void *memset(void *dest, char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern int strcmp(char *str1, char *str2);
extern void strcpy(char *str1, char *str2);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);


/* console.c */
extern void init_console(void);
extern void set_text_attr(unsigned char attr);
extern void move_cursor (void);
extern void k_putchar(unsigned char c);
extern void k_puts(char *string);
extern void scroll_screen();
extern void k_clear_screen();
extern int k_printf(const char *format, ...);

/* gdt.c */
extern void set_gdt(char offset, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags);
extern void init_gdt();
extern void gdt_load();    // in start.asm

/* idt.c */
extern void set_idt(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);
extern void init_idt();
extern void idt_load();    // in start.asm


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void default_handler();

/* pic.h */
extern void init_pic();

/* pit.h */

#define PIT_FREQUNCY 100
extern void init_pit();
extern void pit_ISR();
extern unsigned short int pit_set_tick_count (unsigned short int i) ;
extern unsigned short int pit_get_tick_count ();
extern void start_pit_counter () ;
extern int pit_is_initialized (); 
extern void  pit_handler () ;
extern void init_pit();
extern void set_floppy1c_flag();
extern void reset_floppy1c_flag();
extern void delay(int sec);


/* keyboard.c */
extern void init_keyboard();
extern void keyb_ISR();
extern void setleds();
extern void init_keyboard();
extern void keyb_handler();
extern unsigned char k_getch();
extern unsigned char kbhit();
extern void k_gets(char str[]);
extern int  get_scan_code();


/*dma.c */
void _dma_xfer(unsigned char DMA_channel, unsigned char page, unsigned int offset, unsigned int length, unsigned char mode);
void dma_xfer(unsigned char channel, unsigned long address, unsigned int length, unsigned char read);
void _dma_xfer(unsigned char DMA_channel, unsigned char page, unsigned int offset, unsigned int length, unsigned char mode);

/*floppy.c */
struct DrvGeom_tag 
{
   unsigned char heads;
   unsigned char tracks;
   unsigned char spt;     /* sectors per track */
};
typedef struct DrvGeom_tag DrvGeom ;   

extern void init_floppy();
extern void sendbyte(int byte);
extern int getbyte();
extern int waitfdc(int sensei);
extern void irq6();
extern void int1c();
extern void block2hts(int block,int *head,int *track,int *sector);
extern void floppydisk_reset(void);
extern int diskchange(void);
extern void motoron(void);
extern void motoroff(void);
extern void recalibrate(void);
extern int seek(int track);
extern int log_disk(DrvGeom *g);
extern int read_block(int block,unsigned char *blockbuff, unsigned long nosectors);
extern int write_block(int block,unsigned char *blockbuff, unsigned long nosectors);
extern int fdc_rw(int block,unsigned char *blockbuff,int read,unsigned long nosectors);
extern int format_track(unsigned char track,DrvGeom *g);
extern void _int1c();
extern void floppy_ISR();

/* command.c */
extern void dancing_LED();
extern void reboot_computer();

/* filemanager.c */

extern void parsefilename(char key_filename[13], char root_entry_buffer[32]);
extern void dir();
extern void load_file();

#endif
