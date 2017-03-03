#include "system.h"

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })


#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_start(VAR, LASTARG) 						\
 (VAR = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))

#define va_end(x)
//nothing

#define va_arg(VAR, TYPE)						\
 (VAR += __va_rounded_size (TYPE),					\
  *((TYPE *) (VAR - __va_rounded_size (TYPE))))

#define is_digit(c)	((c) >= '0' && (c) <= '9')

unsigned short *vidmem;
unsigned int attribute;
int cur_x, cur_y;

/* Initialize the VGA memory. It is mapped into 0xB8000 address of RAM. */
void init_console(void)
{
  vidmem =(unsigned short *) 0xB8000;
  attribute = 0x0B;
  cur_x = cur_y = 0;
}

/* It sets the attribute of the character to be printed */
void set_text_attr(unsigned char attr)
{
  attribute = attr;
}

/* This function moves the cursor */
void move_cursor (void)
{
  unsigned int temp;
  temp = (cur_y * 80) + cur_x;
  
  outportb(0x3d4, 14);
  outportb(0x3d5, temp >> 8);
  outportb(0x3d4, 15);
  outportb(0x3d5, temp);
}

void k_putchar(unsigned char c)
{
    unsigned short *location;
    unsigned att = attribute << 8;
    
    
    if(c == '\b')
    {
        if(0 == cur_x && 0==cur_y) 
                 ;                /* do nothing */
        else
        if(cur_x > 0)
	   cur_x--;
        else
        {
          cur_x=79;
          cur_y --;
        }
        location = vidmem + (cur_y * 80 + cur_x);
	*location = ' ' | att;
    }

    else if(c == '\t')
    {
        cur_x += 8;
	cur_x = cur_x > 80 ? 80 : cur_x;
    }

    else if(c == '\r')
    {
        cur_x = 0;
    }

    else if(c == '\n')
    {
        cur_x = 0;
        cur_y++;
        scroll_screen();
    }

    else if(c >= ' ')
    {
        location = vidmem + (cur_y * 80 + cur_x);
        *location = c | att;
        cur_x++;
    }

    if(cur_x >= 80)
    {
        cur_x = 0;
        cur_y++;
    }
    scroll_screen();
 
}

void k_puts(char *string)
{
    int i;

    for (i = 0; string[i]!= '\0' ; i++)
    {
        k_putchar(string[i]);
    }
}


/* This function si responsible for scrolling the screen  */
/* It first checks the value of cursor's x and y position */
/* and then scrolls if required 				    */
void scroll_screen()
{
 int i;
 if(cur_y > 24 || (cur_y== 24 && cur_x== 80))
 {
  for(i=0;i<80*25;i++)
   vidmem[i]=vidmem[i+80];
  
  for(i=80*24;i<25*80;i++)
    vidmem[i] = ' '| attribute << 8 ;
  cur_y= 24;
  cur_x= 0;
  
 }
 move_cursor();
}

/* clears the screen. It means printing blank space through */
/* the whole memory with attribute background colour. 		*/
void k_clear_screen()
{
  unsigned int i;
  for(i=0;i<80*25; i++)
      vidmem[i]=' '| attribute << 8 ;
  
  cur_x = cur_y= 0;
  move_cursor();
  
}

//  Prints a message to the screen
int k_printf(const char *format, ...)
{
        char buffer[512];
        char* arg;
        int i = 0;

        va_start(arg, format);
        vsprintf(buffer, format, arg);
        va_end(arg);


        while(buffer[i] != '\0')
            k_putchar(buffer[i++]);
        return i;
}


static int skip_atoi(const char **s)
{
	int i=0;

	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

static char * number(char * str, int num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
	if (type&LEFT) type &= ~ZEROPAD;
	if (base<2 || base>36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	if (type&SIGN && num<0) {
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	if (sign) size--;
	if (type&SPECIAL)
		if (base==16) size -= 2;
		else if (base==8) size--;
	i=0;
	if (num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if (i>precision) precision=i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type&SPECIAL)
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT))
		while(size-->0)
			*str++ = c;
	while(i<precision--)
		*str++ = '0';
	while(i-->0)
		*str++ = tmp[i];
	while(size-->0)
		*str++ = ' ';
	return str;
}

int vsprintf(char *buf, const char *fmt, char* args)
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

		case 's':
			s = va_arg(args, char *);
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';
	return str-buf;
}

int sprintf(char *buffer, const char *fmt, ...)
{
	int i;
	char *args;

        va_start(args, fmt);
        i = vsprintf(buffer, fmt, args);
        va_end(args);
        return i;
}

void setcolor()
{
  char col, *str;
  if( getargc() == 1)
  {
     k_printf("\n Enter new font color: ");
     col = k_getw();
  }
  else
  {  
      getargv(str,1);
      col= cut_str(str);
  }

  attribute = (attribute & 0xF0) | col ;
}

void setbackground()
{
  char col, *str;
  if( getargc() == 1)
  {
     k_printf("\n Enter new background color: ");
     col = k_getw();
  }
  else
  {
     getargv(str,1);
     col = cut_str(str);
  }

   attribute = (attribute & 0x0F) | (col<< 4)  ;
}

