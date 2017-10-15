#include <stdio.h>

int main()
{
	extern readc();
	extern printc();

  char numeros[10];


	int i;
	int size;
	int key;
	int  j;
	size = 5;

	printc(' ');


	for ( i = 0; i < size; i++) {
		numeros[i] = readc();
		printc(numeros[i]);
	}

	printc(' ');


	for (i = 1; i < size; i++)
	{
		key = numeros[i];
		j = i-1;


		while (j >= 0 && numeros[j] > key)
		{
			 numeros[j+1] = numeros[j];
			 j = j-1;
		}
		numeros[j+1] = key;
	}

	for ( i = 0; i < size; i++) {
		printc(numeros[i]);
	}



  readc();
}
