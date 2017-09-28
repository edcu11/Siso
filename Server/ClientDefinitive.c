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
// TAMANO DE CADA MENSAJE

#ifndef USER_SIZE
#define USER_SIZE 16
#endif
//TAMANO DE NOMBRE DE USUARIO

#ifndef MESSAGE_COUNT
#define MESSAGE_COUNT 8
#endif
//CANTIDAD DE MENSAJES POR USUARIO

#ifndef SHM_SIZE
#define SHM_SIZE 4096
#endif
//CANTIDAD DE MEMORIA ALOCADA


#ifndef INPUT_SIZE
#define INPUT_SIZE 1024
#endif

//DEVUELVE LA POSICION EN DONDE EMPIEZA EL NOMBRE DEL USUARIO BUSCADO
int FindUser(char *shmaddr, char * username)
{
  int x = 0;

  while(x < SHM_SIZE)
  {

    if(strcmp(&shmaddr[x], username) == 0)
    {
    //  printf("Encontrado en %d \n", x );
      return x;
    }

    x += strlen(&shmaddr[x]) + (MESSAGE_SIZE * MESSAGE_COUNT); //AUMENTAMOS X DE ESTA FORMA PARA SALTARNOS EL BLOQUE RESPECTIVO DE CADA USUARIO
                //NOMBREUSUARIO(16) + (48) * (8)

  }

  return -1;
}

void GetInput(char * temp)
{
  fgets(temp, INPUT_SIZE, stdin);
  temp[strcspn(temp, "\n")] = '\0';
}

void GetInputWEnd(char * temp)
{
  fgets(temp, INPUT_SIZE, stdin);
  temp[strcspn(temp, "\n")] = '\0';
}


int Listen(char * shmaddr, int pos)
{
  printf("Waiting... %d\n", pos );

  int x = 0;
  int waitingTime = 400000000;
  while (strcmp(&shmaddr[pos], "") == 0)
  {
      if(x > waitingTime)
      {
        printf("Waiting...\n" );
        x = 0;
      }
      x++;
  }

  return 0;

}



int main(int argsc, char * args[])
{

  int shared_mem_key = 555555;
  int argc = 0;
  char *temp = malloc(INPUT_SIZE);
  char *user = malloc(INPUT_SIZE);
  strcpy(user, "");


  int shm_id = shmget(shared_mem_key, SHM_SIZE, S_IRUSR | S_IWUSR);  //MEMORIA
  char * shmaddr = (char *) shmat(shm_id, NULL, 0);

  while (1)
  {
    printf("Messenger#%s: ",  user);
    GetInput(temp);

    if(strcmp(temp, "createuser") == 0)
    {
      printf("Messenger# crear usuario: " );
      strcpy(&shmaddr[0], temp);
      Listen(shmaddr, 0);

      printf("Nombre de usuario : " );
      GetInputWEnd(temp);
      strcpy(&shmaddr[0], temp);

      Listen(shmaddr, 16);
      printf("Response: %s\n", &shmaddr[16]);
    }

    if(strcmp(temp, "readfromX") == 0 )
    {
      printf("Messenger# readfromX: " );
      GetInput(temp);
      printf("%s\n", &shmaddr[atoi(temp)] );
    }


    if(strcmp(temp, "login") == 0)
    {
      printf("Messenger# usuario: " );
      strcpy(&shmaddr[0], temp);

      GetInput(temp);
      strcpy(&shmaddr[0], temp);
      strcpy(user, temp);

      Listen(shmaddr, 32);
      printf("response %s\n", &shmaddr[32]);


      if( strcmp(&shmaddr[32], "No existe usuario") == 0)
      {
        strcpy(user,"");
      }

    }

    if(strcmp(temp, "write") == 0)
    {
      if(strcmp(user, "") == 0)
      {
        printf("Inicie sesion primero\n");
        continue;
      }

      strcpy(&shmaddr[0], temp);
      printf("Messenger# write to: " );
      GetInputWEnd(temp);
      strcpy(&shmaddr[0], temp);

      Listen(shmaddr, 48);

      if(strcmp(&shmaddr[48], "Destinatario no existe") == 0)
      {
        printf("%s\n",&shmaddr[48]);
        continue;
      }

      printf("Messenger# message: " );
      GetInput(temp);
      strcpy(&shmaddr[80], user);
      strcpy(&shmaddr[96], temp);
    }

    if(strcmp(temp, "read") == 0)
    {
      strcpy(&shmaddr[0], temp);

      Listen(shmaddr, 100);
      strcpy(&shmaddr[100], "\0");


      strcpy(&shmaddr[0], user);

      Listen(shmaddr, 16);

      if(strcmp(&shmaddr[16], "Usuario no se encontro") == 0)
      {
        printf("%s\n",&shmaddr[16]);
        continue;
      }

      printf("Mis mensajes: %s\n", user );

      int i = 0;
      int bufferPos = 16;
      for( ; i < 10 ; i++)
      {
        printf("%s\n", &shmaddr[bufferPos]);
        bufferPos += strlen(&shmaddr[bufferPos]);
      }



    }





  }


















  return 0;
}
