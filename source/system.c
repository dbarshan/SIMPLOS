#include "system.h"

void *memcpy(void *dest, const void *src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, int count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

int strlen(const char *str)
{
    int retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

int strcmp(char *str1, char *str2)
{
   int i, len1, len2 ;
   len1 = strlen(str1);
   len2 = strlen(str2);

   if( len1 > len2 ) return (str1[len2]) ;
   if( len1 < len2 ) return (-str2[len1]) ;
   if(str1[0] == '\0') return (- str2[0]);

   for(i = 0; str1[i]!= '\0' ; i++)
   {
       if(str1[i] != str2[i]) 
          return (str1[i] - str2[i]); 
   }
  
   return (0);
}

void strcpy(char *dest, char *source)
{
        int i;

	for( i= 0; source[i]!= '\0' ; i++)
	{
                 dest[i] = source[i] ; 
                
	}
  	
        dest[i] = '\0' ;
}

int strncmp(char *s1, char *s2, int n)
{

	if (n == 0)
		return (0);
	do {
		if (*s1 != *s2++)
			return (*(unsigned char *)s1 -
				*(unsigned char *)(s2 - 1));
		if (*s1++ == 0)
			break;
	} while (--n != 0);
	return (0);
}


char * strncpy(char *dst, char *src, int n)
{
	char *s, *d;
        if (n != 0)
        {
		 char *d = dst;
                 char *s = src;

		do {
			if ((*d++ = *s++) == 0) {
				/* NUL pad the remaining n-1 bytes */
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return (dst);
}

char toupper(char ch)
{
  if( ch>= 'a' && ch <= 'z')
          return (ch - 32);
}


char tolower(char ch)
{
  if( ch>= 'A' && ch <= 'Z')
          return (ch + 32);
}


unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
