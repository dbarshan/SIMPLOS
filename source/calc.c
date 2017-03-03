void add()
{
  int i, j ;
  k_printf("\nEnter 1st number (interger): ");
  i = k_getw();
  k_printf("\nEnter 2nd number (interger): ");
  j = k_getw();

  k_printf("%d + %d = %d\n", i, j, i+j );
}

void sub()
{
  int i, j ;
  k_printf("\nEnter 1st number (interger): ");
  i = k_getw();
  k_printf("\nEnter 2nd number (interger): ");
  j = k_getw();

  k_printf("%d - %d = %d\n", i, j, i - j );

}

void mul()
{
  int i, j ;
  k_printf("\nEnter 1st number (interger): ");
  i = k_getw();
  k_printf("\nEnter 2nd number (interger): ");
  j = k_getw();

  k_printf("%d * %d = %d\n", i, j, i*j );
}

void div()
{
  int i, j ;
  k_printf("\nEnter 1st number (interger): ");
  i = k_getw();
  k_printf("\nEnter 2nd number (interger): ");
  j = k_getw();
      
  k_printf("%d / %d = %d\n", i, j, i / j );
}
