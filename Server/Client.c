#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

#ifndef MESSAGE_SIZE
#define MESSAGE_SIZE 48
#endif

#ifndef USER_SIZE
#define USER_SIZE 16
#endif

#ifndef MESSAGE_COUNT
#define MESSAGE_COUNT 8
#endif

#ifndef SHM_SIZE
#define SHM_SIZE 4096
#endif


/*
  ./client.o writefrom _sender_  _dest_  _message_
  ./client.o readfrom _user_
  ./client.o createuser _user_

*/


int FindUser(char *shmaddr, char * username)
{
  int x = 0;

  while(x < SHM_SIZE)
  {

    if(strcmp(&shmaddr[x], username) == 0)
    {
      printf("Encontrado en %d \n", x );
      return x;
    }

    x += strlen(&shmaddr[x]) + (MESSAGE_SIZE * MESSAGE_COUNT);

  }

  return -1;
}



int main(int argc, char ** argv)
{

  int shared_mem_key = 555555;
  int pos_toWrite;
  printf("Cliente Conectado! \n" );

  int shm_id = shmget(shared_mem_key, SHM_SIZE, S_IRUSR | S_IWUSR);
  char * shmaddr = (char *) shmat(shm_id, NULL, 0);

  if(argv[1] == NULL)
  {
    printf("You can use the commands: \n \n");
    printf("writefrom _sender_  _dest_  _message_ \n" );
    printf("readfrom _user_\n" );
    printf("createuser _user_\n \n" );
    return 0;
  }



  if( strcmp(argv[1], "createuser") == 0 && argc > 1)
  {
    int posEncontrada = 0;
    int x= 0;

    if(FindUser(shmaddr, argv[2]) != -1)
    {
      printf("Usuario existe! \n" );
      return 0;
    }

    while (posEncontrada == 0)
    {
      if(strlen(&shmaddr[x]) == 0)
      {
        strcpy(&shmaddr[x], argv[2]);
        printf("Wrote at: %d \n", x );
        return 0;
      }

      x += strlen(&shmaddr[x]) + (MESSAGE_SIZE * MESSAGE_COUNT);
    }

  }


  if( strcmp(argv[1], "readfromX") == 0 && argc > 1)
  {

    printf("%s\n", &shmaddr[atoi(argv[2])] );

  }

  if( strcmp(argv[1], "readfrom") == 0 && argc > 1)
  {
      int bufferInit = FindUser(shmaddr, argv[2]);

      if(bufferInit == -1 )
      {
        printf("Usuario no existe \n" );
        return 0;
      }

      printf("Mensajes de %s \n", &shmaddr[bufferInit] );

      bufferInit += strlen(&shmaddr[bufferInit]) + 1;
      int bufferEnd = bufferInit + (MESSAGE_SIZE * MESSAGE_COUNT);

      for (; bufferInit < bufferEnd; bufferInit += MESSAGE_SIZE)
      {
        int contador = 0;
        while(contador < 32)
        {
          printf("%s ", &shmaddr[bufferInit + contador] );
          contador += strlen(&shmaddr[bufferInit + contador]);
          contador++;
        }
        printf("\n" );
      }

  }



  if( strcmp(argv[1], "writefrom") == 0 && argc > 4)
  {

    if(FindUser(shmaddr, argv[2]) == -1 || FindUser(shmaddr, argv[3]) == -1)
    {
      printf("Uno de los usuarios no existe! \n");
      return 0;
    }

    int bufferInit = FindUser(shmaddr, argv[3]);
    bufferInit += strlen( &shmaddr[FindUser(shmaddr, argv[3])] ) + 1;

    int contadorDeMensajes = 0;
    while(strcmp(&shmaddr[bufferInit], "") != 0)
    {
      bufferInit += MESSAGE_SIZE;
      contadorDeMensajes++;
    }

    if (contadorDeMensajes >= MESSAGE_COUNT)
    {
      bufferInit -= MESSAGE_SIZE * MESSAGE_COUNT;
    }

    int x;
    int bufferTemporal = bufferInit;

    strcat(&shmaddr[bufferTemporal], argv[2]);
    bufferTemporal += strlen(argv[2]);
    strcat(&shmaddr[bufferTemporal], ": ");
    bufferTemporal += 3;

    for (x = 0; x < argc  - 4; x++)
    {
      printf(" buffer %d\n", bufferTemporal );
      printf("%s\n", argv[x + 4] );
      strcat(&shmaddr[bufferTemporal], argv[x + 4]);
      bufferTemporal += strlen(argv[x + 4]) + 1;
    }

    printf("%s\n", &shmaddr[bufferInit]);
    printf("Written at: %d\n", bufferInit );


  }


  return 0;
}
