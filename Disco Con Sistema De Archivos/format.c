 #include <stdio.h>

struct Block {
	int sector;
	int head;
	int track;
};

int main()
{
	extern readc();
	extern printc();
	extern writesect();
	extern readsect();
	extern printInt();

	char  buffer[512];
	int i = 0;
	int BlocksFormatted = 1;
  int actualBlock = 1;
	int BlockAmount = 483;
	int sector = 1;
	int head = 1;
	int track = 0;
	int *buffer_pirateado;

	/*  SUPER BLOCK */
  int libres = 241;
	int isLastSector = 0;

  printc('\n');

	buffer_pirateado = buffer;
	buffer_pirateado[i] = 1;
  buffer_pirateado[i+1] = 1;
  buffer_pirateado[i+2] = 0;
	buffer_pirateado[i+3] = 13;
	buffer_pirateado[i+4] = 1;
  buffer_pirateado[i+5] = 13;
  buffer_pirateado[i+6] = libres;



	writesect(buffer, 18, 0, 0);
	/* END */

	printc('\n');
	for (; BlocksFormatted < BlockAmount; BlocksFormatted++, isLastSector = (isLastSector == 0)? 1 : 0)
	{

		for (i = 0; i < 512; i++) {
			buffer[i] = 'z';
		}

		if(isLastSector)
		{
			buffer_pirateado = buffer + 508;
      buffer_pirateado[0] = actualBlock;
      actualBlock++;
		}

		writesect(buffer, sector, head, track);

		if(sector == 18)
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
		sector++;
	}

  for (i = 0; i < 512; i++) {
    buffer[i] = 0;
  }
  writesect(buffer, 13, 1, 13);
  i = 508;
  buffer[i] = -1;
  writesect(buffer, 14, 1, 13);



  printc('F');
  printc('o');
  printc('r');
  printc('m');
  printc('a');
  printc('t');
  printc('t');
  printc('e');
  printc('d');
  printc('!');
  printc('\n');


	readc();
}


//writesect(buffer, sector, track, side);

/*
[1:34 PM, 9/2/2017] Jonathan Salazar: Los sectores son de 1-18
[1:34 PM, 9/2/2017] Jonathan Salazar: Tracks 0-79
[1:35 PM, 9/2/2017] Jonathan Salazar: Side 0 y 1

*/
