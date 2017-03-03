#include "system.h"


#define bpbBytesPerSector 512
#define bpbSectorsPerCluster 1
#define bpbReservedSectors 1
#define bpbNumberOfFATs 2
#define bpbRootEntries 	224
#define bpbTotalSectors 2880
#define bpbMedia 0xf8  
#define bpbSectorsPerFAT 9
#define bpbSectorsPerTrack 18
#define bpbHeadsPerCylinder 2



void   str_toupper(char search_filename[13])
{
  int i;
  for( i= 0; i < 13;i++)
  {
     if(search_filename[i] >= 'a' && search_filename[i] <='z')
             search_filename[i] = search_filename[i] - 32 ;
   }
 
} 
/*The first 11 bytes form the filename.  extract and store it in key_filename string */
void parsefilename(char key_filename[13], char buffer[ ])
{
   int i,k;
   k= 0;
   for( i= 0 ; i<8;i++)
   {
    if(buffer[i]!= ' ')            //SPACE
    {
       key_filename[k]=buffer[i] ;
       k++; 
    } 
   }

   key_filename[k]= '.' ; k++;
   for( i= 8 ; i<11;i++)
   {
    if(buffer[i]!=' ')
    { 
       key_filename[k]=buffer[i] ;
       k++ ;
    }
    key_filename[k]= '\0' ;
   }
}


/*begin with the position mentioned by start variable. extract 32 byte from 512 byte buffer and store it in root_entry_buffer */
void get_root_entry(char root_entry_buffer[32], char buffer[512], int start)
{
  int j;
  for( j= 0; j< 32 ; j++)
      root_entry_buffer[j] = buffer[start+ j ];
}




void dir()
{

      int ax,cx, i ,j, key_size;
      unsigned char buffer[512], root_entry_buffer[32], buffer2[512];
      unsigned char key_filename[13];
      unsigned short int key_cluster;
       
   

    // compute number of logical blocks for root directory and store in cx
     
	cx= (32 *bpbRootEntries)/ bpbBytesPerSector ;

   // compute starting logical block location of root directory and store in ax
     
	ax = bpbNumberOfFATs* bpbSectorsPerFAT + bpbReservedSectors ; 
        
  // read root directory into buffer
 // Now using first 512 bytes. To do : accessing 512 onwards
             
               read_block(ax, buffer,1);  	     // now 512 bytes from root directory is in buffer
            
              for( i= 0 ; i< 512 ; i+= 32)
              {
                get_root_entry(root_entry_buffer, buffer, i);     // cut 32 byte from 512 byte buffer
                parsefilename(key_filename,root_entry_buffer);    // get filename
                key_cluster = (( root_entry_buffer[27] << 8) & 0xFF00) | root_entry_buffer[26]  ;  // get starting cluster
                key_size    =  root_entry_buffer[31]<< 24 | root_entry_buffer[30] << 16 | root_entry_buffer[29] << 8 | root_entry_buffer[28] ;
                if(key_filename[0]!= 0)
                   k_printf("%12s size= %5d bytes starting cluster= %d\n",key_filename,key_size,key_cluster);
                      
              }
             
              floppydisk_reset();
}


/* load root directory and find the string search_filename in the root table.
 * If found return the first cluster stored with it
 * Else return -1 to denote unsuccessfull search
 */

short int get_key_cluster(char search_filename[13])
{
   int cx, ax , i , j;
   short int key_cluster ;
   unsigned char buffer3[512],buffer4[32];
   unsigned char key_filename[13];

  /* first find the filename in root directory */
  /* compute number of logical blocks for root directory and store in cx */
     
     cx= (32 *bpbRootEntries)/ bpbBytesPerSector ;

 /* compute starting logical block location of root directory and store in ax */
     
     ax = bpbNumberOfFATs* bpbSectorsPerFAT + bpbReservedSectors ; 
     

 /* read root directory into buffer */
 // Now using first 512 bytes. To do : accessing 512 onwards

              read_block(ax, buffer3,1);  	     // 512 byte from root directory is in buffer
              
              for( i= 0 ; i<= 480 ; i+= 32)
              {
                get_root_entry(buffer4, buffer3, i);       // get 32 byte from 512 byte entry
                parsefilename(key_filename,buffer4);       // get filename
                str_toupper(search_filename);              // make the lower case character to upper case
                if(!strcmp(search_filename, key_filename ))
                {
                  key_cluster = ( buffer4[27] << 8)  | buffer4[26]  ; // get starting cluster
                  return (key_cluster);
                } 
              }
              
              floppydisk_reset();
                                     
          
          
        return (-1);
}

void load_file()
{

  char search_filename[13], tempstr[13];
  unsigned short int  next_cluster , i, j, k, m;
  short int key_cluster ;
  unsigned int logical_block, datasector, ax, cx ; 
  unsigned char buffer2[512], buffer[512];
 
  if( getargc() == 1)
  {

     k_printf("\n Enter filename: ");
     k_gets(search_filename);
  }
  else
  {
      getargv(search_filename,1);
  }
 
  strcpy(tempstr, search_filename);
  key_cluster= get_key_cluster(tempstr) ;

  if( key_cluster == -1)
  {
      k_printf("\n %s not found.\n",search_filename); 
      return ;
  }  
  do
  {
     floppydisk_reset();
 
    /* we have got the cluster . so load it */
    datasector = bpbNumberOfFATs* bpbSectorsPerFAT + bpbReservedSectors + ((32 *bpbRootEntries)/ bpbBytesPerSector ) ;
    logical_block = (key_cluster -2) * bpbSectorsPerCluster + datasector ;
  
    read_block( logical_block, buffer2,1);  
    for (k = 0;k <512 ; k++)
      k_printf("%c", buffer2[k]);   
    k_getch();
    /* Now load FAT */
    /* compute size of FAT and store in cx */
    cx=  bpbNumberOfFATs* bpbSectorsPerFAT ;
	
    // compute location of FAT and store in ax
    ax = bpbReservedSectors ;
    
    /* Now buffer holds the 512 bytes from FAT */
    read_block(ax,buffer ,1);
    
    /* compute next cluster */
    m = (key_cluster/2) + key_cluster;
    next_cluster = buffer[m]| (buffer[m+1] << 8);   // FAT entry - m'th byte lsb.  m+1 th byte msb 

    if(key_cluster %2)                // if odd cluster
    {
      next_cluster = next_cluster >> 4;
    }
    else                             // if even cluster
    {
      next_cluster =   next_cluster & 0x0FFF;
    }
    
    key_cluster = next_cluster ;

   }while( next_cluster != 4095);  //end cluster*/
 
}

