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



int main(int argsc, char * args[])
{

  int shared_mem_key = 555555;
  int pos_toWrite;
  printf("Cliente Conectado! \n" );

  int i;
  int argc = 0;
//  char **argv = { 0 };
  char *temp = { 0 };
  char *sentence = { 0 };
  char *user = args[1];
  printf("%s\n", &user[0] );




  int shm_id = shmget(shared_mem_key, SHM_SIZE, S_IRUSR | S_IWUSR);  //MEMORIA
  char * shmaddr = (char *) shmat(shm_id, NULL, 0);

  char * backUser = user;
  char * backaddr = shmaddr;
  //BACKUPS USADOS PARA EVITAR QUE SE CORROMPA EL USUARIO
  //DESPUES DE UN READ O WRITE

  while (1) {
    /* code */
  user = backUser;
  shmaddr = backaddr;
  //REPONEMOS LOS BACKUPS
  printf("Messenger#" );
  scanf("%s", &temp);

  if(strcmp(&temp, "exit") == 0)
  {
    return 0;
    //DO I REALLY HAVE TO COMMENT THIS?
  }

  if(strcmp(&temp, "createuser") == 0 )
  {

    printf("Messenger# crear usuario: " );
    int posEncontrada = 0;
    int x= 0;
    int detener = 0;

    scanf("%s", &temp);
    //PEDIMOS NOMBRE DE USUARIO A CREAR

    if(FindUser(shmaddr, &temp) != -1)
    {
      printf("Usuario existe! \n" );
      continue;
    }
    //SI YA EXISTE REGRESAMMOS

    while (posEncontrada == 0 || x < SHM_SIZE)
    {
      if (detener == 1) {
        break;
      }
      //MIENTRAS LA POSICION EN LA QUE ESTAMOS NO SEA UNA STRING VACIO
      // SEGUIREMOS BUSCANDO EN EL SIGUEINTE BLOQUE
      if(strlen(&shmaddr[x]) == 0)
      {
        strcpy(&shmaddr[x], &temp);
        printf("Wrote at: %d \n", x );
        detener = 1;
      }

      x += strlen(&shmaddr[x]) + (MESSAGE_SIZE * MESSAGE_COUNT);
    }

  }

  //FUNCION PARA LEER EN CUALQUIER PARTE DE LA MEMORIA, CAUSE BUGSEVERYWHERE
  if(strcmp(&temp, "readfromX") == 0 )
  {
    printf("Messenger# readfromX: " );
    scanf("%s", &temp);

    printf("%s\n", &shmaddr[atoi(&temp)] );

  }

  //LEEREMOS LOS MENSAJES DE USUARIO
  if(strcmp(&temp, "read") == 0 )
  {
      int bufferInit = FindUser(shmaddr, user);
      //CONFIRMAMOS QUE EXISTA
      if(bufferInit == -1 )
      {
        printf("Usuario no existe \n" );
        continue;
      }
      //HEADER AND GOOOAAAL
      printf("Mensajes de %s \n", &shmaddr[bufferInit] );

      bufferInit += strlen(&shmaddr[bufferInit]) + 1;
      int bufferEnd = bufferInit + (MESSAGE_SIZE * MESSAGE_COUNT);
      //RECORREREMOS HASTA AL FINAL DEL BLOQUE DE MENSAJES
      for (; bufferInit < bufferEnd; bufferInit += MESSAGE_SIZE)
      {
        int contador = 0;
        //IMPRIMIREMOS CADA STRING QUE ENCONTREMOS
        while(contador < 32)
        {
          //EN MEMORIA CADA STRING ESTA PEGADO, POR ESO DEJAMOS UN
          // "%s(notese el espacio)" PARA SEPARAR LAS WORDS
          printf("%s ", &shmaddr[bufferInit + contador] );
          contador += strlen(&shmaddr[bufferInit + contador]);
          contador++;
        }
        printf("\n" ); //ESPACIO ENTRE MENSAJES
      }

  }


  if( strcmp(&temp, "send") == 0)
  {
    //IHATETHISSHIT
    printf("Messenger# writeTo :" );
    scanf("%s", &temp);

    //CONFIRMAMOS QUE AMBOS USUARIOS EXISTAN
    if(FindUser(shmaddr, &temp) == -1 || FindUser(shmaddr, user) == -1)
    {
      printf("Uno de los usuarios no existe! \n");
      return 0;
    }

    //NOS UBICAMOS JUSTO AL PRINCIPIO DEL NOMRBE DEL USUARIO
    int bufferInit = FindUser(shmaddr, &temp);
    bufferInit += strlen( &shmaddr[FindUser(shmaddr, &temp)] ) + 1;
    // + 1 PARA QUE HAYA UN ESPACIO ENTRE USUARIO Y MENSAJES
    // DE LO CONTRARIO NO ENCONTRARIAMOS EL USUARIO PORQUE NO HABRIA FINAL DEL LINA
    //HASTA EL FINAL DEL MENSAJE

    int contadorDeMensajes = 0;
    //NOS MOVEMOS HASTA LA ULTIMA POSICION DE MENSAJE DISPONIBLE
    while(strcmp(&shmaddr[bufferInit], "") != 0)
    {
      bufferInit += MESSAGE_SIZE;
      contadorDeMensajes++;
    }

    //SI NOS PASAMOS DE LA CANTIDAD DE MENSAJES, YA ESTAMOS
    //EN EL ESPACIO DE OTRO USER
    if (contadorDeMensajes >= MESSAGE_COUNT)
    {
      bufferInit -= MESSAGE_SIZE * MESSAGE_COUNT;
    }

    int x;
    int bufferTemporal = bufferInit;

    //COPIAMOS EL USERNAME DEL EMISOR
    strcat(&shmaddr[bufferTemporal], &user[0]);
    bufferTemporal += strlen(&user[0]);
    strcat(&shmaddr[bufferTemporal], ": ");
    bufferTemporal += 3;

    //%[^\n] LE DICE AL COMPI QUE META TODO A UN STRING HASTA QUE ENCUENTRE
    //UN SALTO DE LINEA. AL PARECER SCANF ES UN DELIMITER LOL
    scanf(" %[^\n]", &temp);
    strcat(&shmaddr[bufferTemporal], &temp);


    //PURO DEBUGGERPERRO
    printf("Written at: %d\n", bufferInit );

  }

}

  return 0;
}
