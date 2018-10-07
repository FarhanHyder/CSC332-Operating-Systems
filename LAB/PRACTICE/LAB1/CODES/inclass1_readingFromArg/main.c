#include <stdio.h>
#include <stdlib.h>

int add(int a, int b)
{
  return a+b;
}

int main(int argc, char **argv)
{
  int a,b,c;

  a = atoi(argv[1]);
  b = atoi(argv[2]);

  char buffer[100];
  gets(buffer);
  puts(buffer);

  c = add(a,b);

  printf("Sum of %d+%d=%d\n",a,b,c);
  exit(0);
}
