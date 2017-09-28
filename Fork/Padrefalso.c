#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv)
{


	int x = 1;
	char ** arrayC;
	char *temp = malloc(sizeof(int));
	char *tamanoParticionChar = malloc(sizeof(int));
	int posicionInicial = 1;


	pid_t pid;

	int contadorDeHijos = 0;
	int ParticionEsPar = ((argc - 2) % 2 == 0)? 1 : 0;
	int tamanoParticion = (ParticionEsPar)? 4 : 3;
	int cantidadDeHijosAProCrear = ((argc - 2) / tamanoParticion);

	if(ParticionEsPar)
	{
		cantidadDeHijosAProCrear += 1;
	}

	printf("cantidad de hijos a hacer %d \n Tamano de Particion %d \n ", cantidadDeHijosAProCrear, tamanoParticion);

	while(contadorDeHijos < cantidadDeHijosAProCrear)
	{

		pid = fork();
		contadorDeHijos++;

		if(pid >= 0)
		{
			if(pid == 0)
			{

				char ** arrayC;
				arrayC = malloc((argc	+ 1) * sizeof(char));

				for (x = 1 ; x < argc; x++)
				{
					arrayC[x - 1] = argv[x];
				}

				sprintf(temp, "%d", posicionInicial );
				arrayC[x - 1] = temp;
				sprintf(tamanoParticionChar, "%d", tamanoParticion );
				arrayC[x] = tamanoParticionChar;

				execvp("./hijofalso.o", arrayC);
			}

			else
			{

				printf("padre \n");
			}

		}
		else
		{
			printf("error");
		}

	posicionInicial += tamanoParticion;

	}

	x = 0;
	int wait_result = 0;
	while(x < contadorDeHijos)
	{
		 wait(&wait_result);
		 if(wait_result != 0)
		 {
			 printf(" Encontrado! \n");
			 return 1;
		 }
		 x++;

	}


}
