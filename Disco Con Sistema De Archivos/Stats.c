#include <stdio.h>


int main()
{
	extern readc();
	extern printc();
	extern writesect();
	extern readsect();
	extern printInt();


	char numberInput[3];
	char firstBlockBuffer[512];
	char buffer[512];
	int BlockNumber = 3;
	int sector = 1;
	int head = 1;
	int track = 0;
	int i = 0;
	int size = 0;
	int * bufferInt;


	readsect(firstBlockBuffer, 18,0,0);
	bufferInt = firstBlockBuffer;
	size = *(bufferInt + 6);

	printc('L');
	printc('i');
	printc('b');
	printc('r');
	printc('e');
	printc('s');
	printc(':');
	printc(' ');
	printInt(size);
	printc('\n');
	printc('T');
	printc('a');
	printc('m');
	printc(':');
	printc(' ');
	printInt(241);

	printc('N');

	return 1;
}
