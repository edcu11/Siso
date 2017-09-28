#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {

  key_t shared_mem_key = 555555;
  int shm_id = shmget(shared_mem_key, 4096, IPC_CREAT | S_IRUSR | S_IWUSR);

  if(shm_id == -1)
  {
    return -1;
  }

  if( (argc - 1 )% 8 != 0 || argc == 1 )
  {
    printf("Numero de argumentos invalido! \n" );
    return 0;
  }

  pid_t pid;
  int numbersFrom = 1;
  int tamanoParticion = (argc - 1) / 8;
  int contadorDeHijos = 0;
  int posicionEscribir = 0;
  char *temp = malloc(sizeof(int));

  while (contadorDeHijos < 8)
  {
    pid = fork();
    contadorDeHijos++;
    posicionEscribir++;

    if(pid >= 0)
    {
      if(pid == 0)
      {
        printf("PID de hijo empiezo en  %d \n", numbersFrom );

				char ** arrayC;
        int contador = 0 ;
				arrayC = malloc((tamanoParticion + 1) * sizeof(char));

        while(contador < tamanoParticion)
        {
          arrayC[contador] = malloc(sizeof(int));
          contador++;
        }

        contador = 0;
        int argumentos = numbersFrom;
        while(contador < tamanoParticion)
        {
          printf("estoy copiando %d\n", atoi(argv[argumentos]));
          arrayC[contador] = argv[argumentos++];
          printf("Copie %d\n", atoi(arrayC[contador]) );
          contador++;
        }

        sprintf(temp, "%d", posicionEscribir);
        arrayC[contador - 1] = temp;


        execvp("./bubblesort.o", arrayC);
      }
      else
      {
        printf("PID de padre\n" );
      }

    }
    else
    {
      printf("Error \n" );
    }


    numbersFrom += tamanoParticion;



  }



  return 0;
}
