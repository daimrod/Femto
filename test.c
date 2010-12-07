#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[])
{
  int x = 0xFFFFFFFF;
  printf("%d\n",  x);
  printf("%f\n", (float) x);
  return 0;
}
