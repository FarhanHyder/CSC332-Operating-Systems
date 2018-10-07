#include <stdio.h>

int factorial(int n)
{
  if (n==0)
  {
    return 1;
  }
  return n*factorial(n-1);
}

int main()
{
  int i;
  //run a loop
  for (i=1; i<=10; i++)
  {
    printf("%d! = %d\n",i,factorial(i));
  }
  return 0;
}
