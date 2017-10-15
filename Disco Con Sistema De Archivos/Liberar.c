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
	int nextSector = 0;
	int nextTrack = 0;
	int nextHead = 0;
	int lastSector = 0;
	int lastHead = 0;
	int lastTrack = 0;

	int size = 0;
	int * bufferInt;

	printc('\n');
	numberInput[0] = readc();
	printc(numberInput[0]);
	numberInput[1] = readc();
	printc(numberInput[1]);
	numberInput[2] = readc();
	printc(numberInput[2]);
	BlockNumber = ((numberInput[0]-'0') * 100) + ((numberInput[1]-'0') * 10) + (numberInput[2]-'0');


	readsect(firstBlockBuffer, 18,0,0);
	bufferInt = firstBlockBuffer;
	lastSector = *(bufferInt + 3);
	lastHead = *(bufferInt + 4);
	lastTrack = *(bufferInt + 5);
	size = *(bufferInt + 6);


	for ( i = 1; i < BlockNumber; i++)
	{

			if(sector == 17)
			{
				sector = 1;
				if(head == 0)
				{
					head = 1;
				}
				else
				{
					head = 0;
					track = track + 1;
				}
				continue;
			}
			sector += 2;

	}

	/*printc('\r');
	printc('\n');
	printInt(sector);
	printc('\n');
	printInt(head);
	printc('\n');
	printInt(track);
	printc('\n');*/

	readsect(buffer, sector + 1, head, track);
	bufferInt = buffer + 508;
	nextSector = *(bufferInt + 0);
	printInt(nextSector);

	if(nextSector != 0 )
	{
		if(nextSector != -1 )
		{
		printc('\n');
		printc('\r');
		printc('Y');
		printc('a');
		printc(' ');
		printc('e');
		printc('s');
		printc(' ');
		printc('L');
		printc('i');
		printc('b');
		printc('r');
		printc('e');
		printc('\n');
		return 1;
		}
	}

	for (i = 0; i < 512; i++) {
		buffer[i] = 0;
	}
	i = 508;
	buffer[i] = -1;
	writesect(buffer, sector + 1, head, track);


	printc('L');
	printc('i');
	printc('b');
	printc('e');
	printc('r');
	printc('a');
	printc('d');
	printc('o');
	printc('\n');

	for (i = 0; i < 512; i++) {
		buffer[i] = 0;
	}
	i = 0;

	bufferInt = buffer+508;
	bufferInt[i] = BlockNumber;


	writesect(buffer, lastSector + 1, lastHead, lastTrack);

	readsect(firstBlockBuffer, 18,0,0);
	bufferInt = firstBlockBuffer;

	if(lastSector == 18 && lastHead == 18 && lastTrack == 18)
	{
		bufferInt[i+0] = sector;
		bufferInt[i+1] = head;
		bufferInt[i+2] = track;

	}

	bufferInt[i+3] = sector;
	bufferInt[i+4] = head;
	bufferInt[i+5] = track;
	bufferInt[i+6] = size + 1;
	writesect(firstBlockBuffer, 18, 0, 0);

	printc('N');

	return 1;
}
