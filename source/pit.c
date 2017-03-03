#include "system.h"


static unsigned int pit_ticks = 0;

unsigned int delay_flag;

asm (
   ".globl pit_ISR         \n"
   "pit_ISR:               \n"
   "   pusha               \n" 
   "   pushw %ds           \n" 
   "   pushw %es           \n"
   "   pushw %ss           \n" 
   "   pushw %ss           \n"
   "   popw %ds            \n"
   "   popw %es            \n"
   "                       \n"
   "   call pit_handler    \n"
   "                       \n"
   "   popw %es            \n"
   "   popw %ds            \n" 
   "   popa                \n"
   "   iret                \n" 
);


unsigned short int pit_set_tick_count (unsigned short int i) 
{
        unsigned short int ret;
	ret = pit_ticks;
	pit_ticks = i;
	return ret;
}


//! returns current tick count
unsigned short int pit_get_tick_count ()
{
	return pit_ticks;
}

//! starts a counter
void start_pit_counter ( ) 
{

	unsigned short int  COUNT ;
	COUNT = (1193181 / 1000);

	/* send operational command. counter 0, first read msb then lsb, mode 3 squarewave generator, binary counter */
	outportb(0x43,0x34);

	/* set frequency rate. first lsb then msb. */
	outportb(0x40,COUNT & 0xFF);
	outportb(0x40,(COUNT >> 8) & 0xFF);

	//! reset tick count
	pit_ticks=0;
}



void  pit_handler () 
{

	pit_ticks ++ ;
        if(delay_flag)
        {
	   delay_flag -- ;
        }
        
        /* send EOI signal */
	outportb(0x20,0x20);

}


void init_pit()
{
	set_idt (32, ( unsigned int )pit_ISR,0x08, 0x8E);
	delay_flag= 0;
}

void delay(int ms)
{

  delay_flag = ms ;
  while(delay_flag != 0);
}


