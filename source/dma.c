#include "system.h"


/* Defines for accessing the upper and lower byte of an integer. */
#define LOW_BYTE(x)         (x & 0x00FF)
#define HI_BYTE(x)          ((x & 0xFF00) >> 8)

/* Quick-access registers and ports for each DMA channel. */
unsigned char MaskReg[8]   = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
unsigned char ModeReg[8]   = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
unsigned char ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };

unsigned char PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
unsigned char AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
unsigned char CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

void _dma_xfer(unsigned char DMA_channel, unsigned char page, unsigned int offset, unsigned int length, unsigned char mode);

void dma_xfer(unsigned char channel, unsigned long address, unsigned int length, unsigned char read)
{
	unsigned char page=0, mode=0;
	unsigned int offset = 0;
	
	if(read)
		mode = 0x48 + channel;
	else
		mode = 0x44 + channel;
		
	page = address >> 16;
	offset = address & 0xFFFF;
	length--;
	
	_dma_xfer(channel, page, offset, length, mode);	
}	
		

void _dma_xfer(unsigned char DMA_channel, unsigned char page, unsigned int offset, unsigned int length, unsigned char mode)
{
    /* Don't let anyone else mess up what we're doing. */
    asm("cli");

    /* Set up the DMA channel so we can use it.  This tells the DMA */
    /* that we're going to be using this channel.  (It's masked) */
    outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);

    /* Clear any data transfers that are currently executing. */
    outportb(ClearReg[DMA_channel], 0x00);

    /* Send the specified mode to the DMA. */
    outportb(ModeReg[DMA_channel], mode);

    /* Send the offset address.  The first byte is the low base offset, the */
    /* second byte is the high offset. */
    outportb(AddrPort[DMA_channel], LOW_BYTE(offset));
    outportb(AddrPort[DMA_channel], HI_BYTE(offset));

    /* Send the physical page that the data lies on. */
    outportb(PagePort[DMA_channel], page);

    /* Send the length of the data.  Again, low byte first. */
    outportb(CountPort[DMA_channel], LOW_BYTE(length));
    outportb(CountPort[DMA_channel], HI_BYTE(length));

    /* Ok, we're done.  Enable the DMA channel (clear the mask). */
    outportb(MaskReg[DMA_channel], DMA_channel);

    /* Re-enable interrupts before we leave. */
    asm("sti");
}
