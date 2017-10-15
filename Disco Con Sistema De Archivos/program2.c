#include <stdio.h>

int main()
{
	extern readc();
	extern printc();

  char x;
  printc(' ');

  x = readc();
  printc(x);

  printc(' ');

  printc('E');
  printc('S');

  printc(' ');


  if(x % 2 != 0)
  {
    printc('I');
    printc('M');

  }

  printc('P');
  printc('A');
  printc('R');

  readc();
}
