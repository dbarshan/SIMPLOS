#include "system.h"

#define MAXCOMMANDS 17
char *COMMANDS[MAXCOMMANDS] = {
          			  "ledance",        //0
			          "reboot",         //1
			          "cls",            //2
			          "dir",            //3
			          "read",           //4
			          "date",           //5
			          "time",           //6
			          "setprompt",      //7
                                  "setdate",        //8
                                  "settime",        //9
			          "setcolor",       //10
			          "setbackground",  //11
			          "add",            //12
				  "help",            //13
 				  "sub",
			          "mul",
				  "div"

			      } ;

char *COMMAND_DES[MAXCOMMANDS] = {	
					"numlk cpslk scrollk",
					"reboots system",
					"clears screen",
					"lists directory",
					"reads file",
					"displays current date",
					"displays current time",
                                        "sets new prompt",
					"sets new date",
					"sets new time",
					"sets new font color",
					"sets new background color",
					"adds two numbers",
					"displays list of commands",  
					"subtracts two numbers",
					"multiplies two numbers",
					"divides two numbers"
				   } ;

char argv[30][30];
int argc;

int detect_command(char *command)
{
       int i, j;
       char commandpart[30];

 
       argc = 0; j = 0 ;
       for( i = 0 ; command[i]!='\0'; i++)
       {
             if( command[i] == ' ')
             {
                   argv[argc][j] = '\0' ;
                   argc++ ;
                   j = 0  ;
             }
             else
             {
                   argv[argc][j] = command[i];
                   j++ ;
             }
       } 
        argv[argc][j] = '\0' ; 
        argc++; 
        
            
       for(i= 0; i< MAXCOMMANDS ; i++ )
          if (!strcmp(argv[0], COMMANDS[i]))
                 return (i);
       return (-1) ;
}

void default_command_handler(char *command)
{
     if(command[0] != '\0' )
         k_printf("%s not recognized", command);

}

void dancing_LED()
{
  char i;
  if( argc >1)
  {
     for( i =0 ;i<argc-1; i++)
        k_printf("%s ",argv[i]);
     default_command_handler(argv[argc-1]); 
     return;
  }
  while(!kbhit()) 
  { 
    for( i =0 ; i<=7; i++ )
    {
  	outportb(0x60, 0xED);
	while(inportb(0x64) & 2);
	outportb(0x60, i);
	while(inportb(0x64) & 2);
        delay(100);
    }
  }
}

void reboot_computer()
{
	int k;
        if( argc >1)
   	{
   	  for( k =0 ; k < argc-1; k ++)
   	     k_printf("%s ",argv[k]);
   	  default_command_handler(argv[argc-1]); 
   	  return;
  	}
	outportb(0x3f2,0x00);
	outportb(0x64,0xfe);
	for (k=0; k<4000; k++);
   	    outportb(0x64,0xff);
}

void date()
{
  unsigned char ch;
  int i ;
  if( argc >1)
  {
     for( i =0 ;i<argc-1; i++)
        k_printf("%s ",argv[i]);
     default_command_handler(argv[argc-1]); 
     return;
  }

  outportb(0x70, 0x0b);
  ch = inportb(0x71);
  ch = ch & 0x04 ;
  k_printf("Current date : ");
  if(ch == 1)   // binary mode
  {
      outportb(0x70, 0x07);
      ch = inportb(0x71);
      k_printf("%02d/",ch);
      outportb(0x70, 0x08);
      ch = inportb(0x71);
      k_printf("%02d/",ch);
      outportb(0x70, 0x32);
      ch = inportb(0x71);
      k_printf("%02d",ch);
      outportb(0x70, 0x09);
      ch = inportb(0x71);
      k_printf("%02d",ch);
  }
  else          // BCD mode
  {

      outportb(0x70, 0x07);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d/",ch);
      outportb(0x70, 0x08);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d/",ch);
      outportb(0x70, 0x32);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d",ch);
      outportb(0x70, 0x09);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d\n",ch);
      
 }
}

void time()
{
  unsigned char ch ;
  int i;
  if( argc >1)
  {
     for( i =0 ;i<argc-1; i++)
       	 k_printf("%s ",argv[i]);
     default_command_handler(argv[argc-1]); 
     return;
  }


  outportb(0x70, 0x0b);
  ch = inportb(0x71);
  ch = ch & 0x04 ;
  k_printf("Current time : ");
  if(ch == 1)   // binary mode
  {
      outportb(0x70, 0x04);
      ch = inportb(0x71);
      k_printf("%02d:",ch);
      outportb(0x70, 0x02);
      ch = inportb(0x71);
      k_printf("%02d:",ch);
      outportb(0x70, 0x0);
      ch = inportb(0x71);
      k_printf("%02d\n",ch);
  }
  else          // BCD mode
  {
      outportb(0x70, 0x04);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d:",ch);
      outportb(0x70, 0x02);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d:",ch);
      outportb(0x70, 0x0);
      ch = (inportb(0x71) >> 4) *10 + (inportb(0x71) & 0x0F);
      k_printf("%02d\n",ch);
      
 }
}

void setdate()
{
  unsigned char ch ;
  char cur_date[10];
  if( argc == 1)
  {
    k_printf("\n Enter new date (dd/mm/yyyy) : ");
    k_gets(cur_date);
    
  }
  else
  {
      strcpy(cur_date,argv[1]);
  }
 


  outportb(0x70, 0x0b);
  ch = inportb(0x71);
  ch = ch & 0x04 ;
  
  if(ch == 1)   // binary mode
  {
      outportb(0x70, 0x07);
      ch = cur_date[0]*10 + cur_date[1] ;
      outportb(0x71, ch);
      outportb(0x70, 0x08);
      ch = cur_date[3]*10 + cur_date[4] ;
      outportb(0x71,ch);
      outportb(0x70, 0x32);
      ch = cur_date[6]*10 + cur_date[7] ;
      outportb(0x71,ch);
      outportb(0x70, 0x9);
      ch = cur_date[8]*10 + cur_date[9] ;
      outportb(0x71,ch);
  }
  else          // BCD mode
  {
      outportb(0x70, 0x07);
      ch =( (cur_date[0] - '0')<< 4)|(cur_date[1]- '0') ;
      outportb(0x71,ch);
      outportb(0x70, 0x08);
      ch =( (cur_date[3] - '0')<< 4)|(cur_date[4]- '0') ;
      outportb(0x71,ch);
      outportb(0x70, 0x32);
      ch =( (cur_date[6] - '0')<< 4)|(cur_date[7]- '0') ;
      outportb(0x71,ch);
      outportb(0x70, 0x9);
      ch =( (cur_date[8] - '0')<< 4)|(cur_date[9]- '0') ;
      outportb(0x71,ch);
  }
  k_printf("\n Changes Saved. \n");
  argc = 1;
  date();
}



void settime()
{
  unsigned char ch ;
  char cur_time[10];


   if(argc == 1)
   {
   	 k_printf("\n Enter new time (hh:mm:ss) : ");
  	 k_gets(cur_time);
    
  }
  else
  {
         strcpy(cur_time,argv[1]);
  }
 
  outportb(0x70, 0x0b);
  ch = inportb(0x71);
  ch = ch & 0x04 ;
  
  if(ch == 1)   // binary mode
  {
      outportb(0x70, 0x04);
      ch = cur_time[0]*10 + cur_time[1] ;
      outportb(0x71, ch);
      outportb(0x70, 0x02);
      ch = cur_time[3]*10 + cur_time[4] ;
      outportb(0x71,ch);
      outportb(0x70, 0x0);
      ch = cur_time[6]*10 + cur_time[7] ;
      outportb(0x71,ch);
  }
  else          // BCD mode
  {
      outportb(0x70, 0x04);
      ch =( (cur_time[0] - '0')<< 4)|(cur_time[1]- '0') ;
      outportb(0x71,ch);
      outportb(0x70, 0x02);
      ch =( (cur_time[3] - '0')<< 4)|(cur_time[4]- '0') ;
      outportb(0x71,ch);
      outportb(0x70, 0x0);
      ch =( (cur_time[6] - '0')<< 4)|(cur_time[7]- '0') ;
      outportb(0x71,ch);
 }
  k_printf("\n Changes Saved. \n");
  argc = 1;
  time();
}


void setprompt(char PROMPT[])
{
 char INPUT[30] ;
  if( argc == 1)
  {
	 k_printf("\n Enter new prompt: ");
	 k_gets(INPUT);
  }
  else
  {
      strcpy(INPUT,argv[1]);
  }

 strcpy(PROMPT,INPUT);
}

void help()
{
  int i;
  if( argc >1)
  {
     for( i =0 ;i<argc-1; i++)
        k_printf("%s ",argv[i]);
     default_command_handler(argv[argc-1]); 
     return;
  }

  for(i = 0 ; i <MAXCOMMANDS ; i++)
	k_printf("\n %-15s %10s		%s", COMMANDS[i],"-",COMMAND_DES[i]);
  k_printf("\n");
        
}

int getargc()
{
   return (argc) ;
}


void getargv(char *copy, int num)
{

   strcpy(copy, argv[num]);
}



