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
	int BlocksFormatted = 0;
	int BlockAmount = 3;
	int nextBlock = 0;
	int sector = 1;
	int head = 0;
	int track = 1;
	int i = 0;
	int nextSector = 0;
	int nextTrack = 0;
	int nextHead = 0;
	int lastSector = 0;
	int lastTrack = 0;
	int lastHead = 0;
	int size = 0;
	int * bufferInt;

	printc('\n');
	numberInput[0] = readc();
  printc(numberInput[0]);
  numberInput[1] = readc();
  printc(numberInput[1]);
  numberInput[2] = readc();
  printc(numberInput[2]);
  BlockAmount = ((numberInput[0]-'0') * 100) + ((numberInput[1]-'0') * 10) + (numberInput[2]-'0');

	readsect(firstBlockBuffer, 18,0,0);
	bufferInt = firstBlockBuffer;
	nextSector = *(bufferInt + 0);
	nextHead = *(bufferInt + 1);
	nextTrack = *(bufferInt + 2);
	lastSector = *(bufferInt + 3);
	lastHead = *(bufferInt + 4);
	lastTrack = *(bufferInt + 5);
	size = *(bufferInt + 6);
	printc('\n');
	printInt(nextSector);
	printc('\n');
	printInt(nextHead);
	printc('\n');
	printInt(nextTrack);

	if(size < BlockAmount)
	{
		printc('S');
		printc('i');
		printc('n');
		printc(' ');
		printc('e');
		printc('s');
		printc('p');
		printc('a');
		printc('c');
		printc('i');
		printc('o');
		printc('\n');
		return 1;
	}

		printInt(BlockAmount);
		printc('\n');

	for (BlocksFormatted = 0; BlocksFormatted < BlockAmount; BlocksFormatted++)
	{

		sector = nextSector;
		head = nextHead;
		track = nextTrack;

		readsect(buffer, sector + 1, head, track);
		bufferInt = buffer + 508;
		nextBlock = *(bufferInt + 0);


		printc('L');
		printInt(nextBlock);
		printc('\n');


		if(nextBlock == 255 )
		{
			printc('\n');
			printc('F');
			printc('u');
			printc('l');
			printc('l');
			printc('\n');

			nextSector = 18;
			nextHead = 18;
			nextTrack = 18;

			break;
		}

		for (i = 0, nextSector =1, nextHead= 1, nextTrack = 0; i < nextBlock; i++)
		{
				if(nextSector == 17)
				{
					nextSector = 1;
					if(nextHead == 0)
					{
						nextHead = 1;
					}
					else
					{
						nextHead = 0;
						nextTrack = nextTrack + 1;
					}
					continue;
				}
				nextSector += 2;
		}

		for (i = 0; i < 512; i++) {
			buffer[i] = 0;
		}

		writesect(buffer, sector, head, track);
		writesect(buffer, sector+1, head, track);
	}



	bufferInt = firstBlockBuffer;
	bufferInt[0] = nextSector;
	bufferInt[1] = nextHead;
	bufferInt[2] = nextTrack;
	if(nextBlock == 255)
	{
		bufferInt[3] = nextSector;
		bufferInt[4] = nextHead;
		bufferInt[5] = nextTrack;

	}
	bufferInt[6] = size - BlockAmount;

	writesect(firstBlockBuffer, 18, 0, 0);

	printc('A');
  printc('L');
  printc('o');
  printc('C');
  printc('a');
  printc('D');
  printc('o');
  printc('\n');


	return 1;
}
