
#include "system.h"

char *argv[30]; 
int   argc ;

int k_main()
{

    char command[512], PROMPT1[30], PROMPT[30]  ;
    char ch;
    int i;
    
    init_console();
    set_text_attr(0x0A);
    k_clear_screen();
    k_printf("Initializing system\n");
    k_printf("********************************************************************************");
    k_printf("\n Installing GDT                                             :");
    init_gdt();
    k_printf(" ...[Success]...");
    k_printf("\n Installing IDT 					    :");
    init_idt();
    k_printf(" ...[Success]...");
    k_printf("\n Installing Programmable Interrupt Controller driver [8259] : ");
    init_pic();
    k_printf("...[Success]...");
    k_printf("\n Installing Programmable Interval  Timer      driver [8253] : ");
    init_pit();
    k_printf("...[Success]...");
    start_pit_counter();
   
    __asm__ __volatile("sti\n");
    k_printf("\n Installing Floppy disk driver [8272A]                      : ");
    init_floppy();
    k_printf("                                  : ...[Success]...");
    k_printf("\n Installing Keyboard                                        : ");
    init_keyboard(); 
    k_printf("...[Success]...\n");
    k_printf("******************************Welcome To SIMPLOS******************************");
     
    strcpy(PROMPT,"[brainbarshan@SIMPLOS.PROMPT]# ");
 

    while(1)
    {
       
       k_printf("\n%s",PROMPT);
       k_gets(command);

       
       if( !strcmp(command, "hello"))
       {      
           k_printf("Welcome to SIMPLOS.");
           continue;
       }

       i = detect_command(command);  

       switch(i)
       {
         case 0   :   dancing_LED();		  	   break ;
	 case 1   :   reboot_computer();		   break ;
         case 2   :   k_clear_screen();			   break ;
         case 3   :   dir();				   break ;
         case 4   :   load_file();			   break ;
         case 5   :   date();				   break ;
         case 6   :   time();				   break ;
         case 7   :   setprompt(PROMPT);		   break ;
         case 8   :   setdate();			   break ;
         case 9   :   settime();		           break ;
         case 10  :   setcolor();	 	           break ;
         case 11  :   setbackground();	 	           break ;
         case 12  :   add();			           break ;
         case 13  :   help();                              break ;
         case 14  :   sub();			           break ;
         case 15  :   mul();			           break ;
         case 16  :   div();			           break ;

    
         default  :   default_command_handler(command);
       }

    }
    return 0;
}
