#include <stdio.h>

int print_even_odd(int lower_b, int upper_b);

int main()
{
  print_even_odd(1,10);
}

int print_even_odd(int lower_b, int upper_b)
{
  for(int i=lower_b; i<=upper_b; i++)
  {
    if(i%2 == 0)
      printf("%d is EVEN\n",i);
    else
    printf("%d is ODD\n",i);

  }
}
