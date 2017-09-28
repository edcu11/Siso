#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv[])
{

	printf("iniciando nuevo hijo \n" );
	int x = atoi((char*)argv[argc - 2]);
	int y = x + atoi((char*)argv[argc - 1]);



printf("buscando %d desde %d hasta %d \n", atoi((char*)argv[1]), x , y);



	for (; x < y; x++)
	{
		printf("mirando:  %d \n", atoi((char*)argv[x]) );

		if(atoi((char*)argv[x]) == atoi((char*)argv[0]))
		{
			printf("Encontramos %d en %d\n", atoi((char*)argv[x]), x );
			return 1;
		}

	}

	return 0;
}
