#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>



struct message {
  char * from;
  char * message;
};

struct Cliente
{
  char *name;
  struct message * messages;
  int messagesCount;
} Cliente;


int Listen(char * shmaddr)
{
  int x = 0;
  int waitingTime = 400000000;
  while (strcmp(&shmaddr[0], "") == 0 )
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


int ListenX(char * shmaddr, int pos)
{
  int x = 0;
  int waitingTime = 400000000;
  while (strcmp(&shmaddr[pos], "") == 0 )
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

int FindUser(char *shmaddr,struct Cliente clients[10], int clientscount)
{
  int i;
  printf("Looking for %s\n", &shmaddr[0]);
  for ( i = 0; i < clientscount; i++) {
    if(clients[i].name != NULL)
    if(strcmp(clients[i].name, &shmaddr[0]) ==  0 )
    {
      printf("Encontrado en %d\n", i);
      return i;
    }
  }
  return -1;

}


int FindUserByName(char * name,struct Cliente clients[10], int clientscount)
{
  int i;
  printf("Looking for %s\n", name);
  for ( i = 0; i < clientscount; i++) {
    if(clients[i].name != NULL)
    if(strcmp(clients[i].name, name) ==  0 )
    {
      printf("Encontrado en %d\n", i);
      return i;
    }
  }
  return -1;

}


void CleanSM(char * shmaddr, int pos)
{
  strcpy(&shmaddr[pos], "\0");
}



int main(int argc, char ** argv)
{

  key_t shared_mem_key = 555555;
  int shm_id = shmget(shared_mem_key, 4096, IPC_CREAT | S_IRUSR | S_IWUSR);
  char * temp;
  char * shmaddr = (char *) shmat(shm_id, NULL, 0);
  int lastUserAvailable = 0;

  if(shm_id == -1)
  {
    return -1;
  }

  struct Cliente clients[10];

  CleanSM(shmaddr, 0);  //commands
  CleanSM(shmaddr, 16);  //create user name
  CleanSM(shmaddr, 32); //login as user
  CleanSM(shmaddr, 48); //readfrom user
  CleanSM(shmaddr, 64); //write to user
  CleanSM(shmaddr, 96); //message

  int x = 0;
  for( ; x < 10; x++)
  {
    clients[x].messagesCount = 0;
    clients[x].messages = malloc(sizeof(struct message)*10);
  }



  printf("%s\n", &shmaddr[0]);

  while (1) {


    Listen(shmaddr);
  //  printf("Done waiting\n" );

    if(strcmp(&shmaddr[0], "createuser") == 0)
    {
      printf("Lets create\n");
      CleanSM(shmaddr, 0);
      CleanSM(shmaddr, 16);

      Listen(shmaddr);

      int newUserAt = FindUser(shmaddr, clients, lastUserAvailable);
      if(lastUserAvailable < 10 &&  newUserAt == -1 )
      {
        clients[lastUserAvailable].name = malloc(24);
        strncpy(clients[lastUserAvailable].name , &shmaddr[0], 24);
        printf("Usuario %s creado exitosamente\n", clients[lastUserAvailable].name );
        lastUserAvailable++;
        strcpy(&shmaddr[16], "Creado");
      }
      else
      {
        printf("Usuario ya existe\n" );
        strcpy(&shmaddr[16], "Usuario ya existe");
      }

    }

    if(strcmp(&shmaddr[0], "login") == 0)
    {
      printf("Login...\n" );
      CleanSM(shmaddr, 0);
      CleanSM(shmaddr, 32);

      Listen(shmaddr);

      int userExists = FindUser(shmaddr, clients, lastUserAvailable);
      printf("%d\n", userExists);

      if(userExists == -1)
      {
        strcpy(&shmaddr[32], "No existe usuario");
      }
      else{
        strcpy(&shmaddr[32], "Logged in");
      }
      continue;
    }

    if(strcmp(&shmaddr[0], "write") == 0)
    {
      printf("Writing...\n");
      CleanSM(shmaddr, 48);
      CleanSM(shmaddr, 0);
      CleanSM(shmaddr, 96);

      Listen(shmaddr);

      int writeTo = FindUser(shmaddr, clients, lastUserAvailable);
      printf("%d\n", writeTo );
      if(writeTo == -1)
      {
        strcpy(&shmaddr[48], "Destinatario no existe");
        continue;
      }
      else
      {
        strcpy(&shmaddr[48], "Escriba su mensaje");
      }
      printf("starting lsite\n" );
      ListenX(shmaddr, 96);

      printf("about to write\n" );

      clients[writeTo].messages[clients[writeTo].messagesCount].from = malloc(16);
      strcpy(clients[writeTo].messages[clients[writeTo].messagesCount].from , &shmaddr[80] );
      printf("%s\n", clients[writeTo].messages[clients[writeTo].messagesCount].from);

      clients[writeTo].messages[clients[writeTo].messagesCount].message = malloc( strlen(&shmaddr[96]) );
      strcpy(clients[writeTo].messages[clients[writeTo].messagesCount].message , &shmaddr[96]);
      printf("%s\n", clients[writeTo].messages[clients[writeTo].messagesCount].message );

      if(clients[writeTo].messagesCount++ > 10)
      {
        clients[writeTo].messagesCount = 0;
        printf("Siguiente mensajea sobreescribira!\n");
      }

    }

    if(strcmp(&shmaddr[0], "read") == 0)
    {
      CleanSM(shmaddr,0);
      CleanSM(shmaddr,8);
      CleanSM(shmaddr,16);

      strcpy(&shmaddr[100], "ready");


      int UsertoRead = FindUser(shmaddr, clients, lastUserAvailable);

      if(UsertoRead == -1)
      {
        printf("No se encontro \n" );
        strcpy(&shmaddr[16], "Usuario no se encontro");
        continue;
      }

      if(clients[UsertoRead].messagesCount == 0)
      {
        strcpy(&shmaddr[16], "No hay mensajes aun!");
        continue;
      }

      int i =0;
      int bufferInit = 16;
      for ( ; i < clients[UsertoRead].messagesCount; i++)
      {

        strcpy(&shmaddr[bufferInit], clients[UsertoRead].messages[i].from);
        bufferInit += strlen(clients[UsertoRead].messages[i].from);
        strcpy(&shmaddr[bufferInit], "-" );
        bufferInit += 1;
        strcpy(&shmaddr[bufferInit], clients[UsertoRead].messages[i].message);
        bufferInit += strlen(clients[UsertoRead].messages[i].message);
      }


    }

  }









  return 0;

}
