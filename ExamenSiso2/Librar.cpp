#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <math.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <unistd.h>
typedef int (*writeFile)(int , char*, char * );
typedef int (*readFile)(int , char*, char * );


#ifndef BasicFileSystemPath
#define BasicFileSystemPath "/vagrant/Siso/ExamenSiso2/BasicFileSystem.so"
#endif




void * GetBasicFileSystemObject()
{
  void* sharedObj = dlopen(BasicFileSystemPath, RTLD_LAZY);

  if(sharedObj == NULL)
  {
    printf("ERROR!\n");
    return 0;
  }

  return sharedObj;
}


int main(int argc, char const *argv[])
{

  void *  sharedObj = GetBasicFileSystemObject();

  if(argc < 2)
  {
    printf("Insuficient parameters: %d \n", argc);
    return 0;
  }

  writeFile writeFileCaller = (writeFile) dlsym(sharedObj, "writeFile");
  readFile readFileCaller = (writeFile) dlsym(sharedObj, "readFile");

  char* bufferInicial = (char *)calloc(512,1);
  char* buffer = (char *)calloc(512,1);
  char* fileName= (char*)argv[1];
  int bloqueeLibrador = atoi(argv[2]);

  int tamanoLibre = 0;
  int inicioDeBloquees = 2;
  int finDeBloquees = 0;
  int nuevoFin = 0;
  int bloqueeOcupado = -1;

  //bloquee a librar esta libre o no
  readFileCaller(bloqueeLibrador, bufferInicial, fileName);
  memcpy( &nuevoFin, &bufferInicial[0], sizeof(int) );

  if(nuevoFin != -1)
  {
    printf("Bloque ya esta libre!\n");
    return 0;
  }

  printf("libramos\n");
  //cargamos apuntador a ultimo
  readFileCaller(1, bufferInicial, fileName );
  memcpy( &tamanoLibre, &bufferInicial[sizeof(int) * 1], sizeof(int) );
  memcpy( &inicioDeBloquees, &bufferInicial[sizeof(int) * 2], sizeof(int) );
  memcpy( &finDeBloquees, &bufferInicial[sizeof(int) * 3], sizeof(int) );


  int viajero = inicioDeBloquees;
  int next = 0;
  int nextNext = 0;

  for (size_t bloqueesLeidos = 0; bloqueesLeidos < tamanoLibre; bloqueesLeidos++) {
    readFileCaller(viajero, buffer, fileName);
    memcpy( &next, &buffer[sizeof(int) * 0], sizeof(int) );

    readFileCaller(next, buffer, fileName);
    memcpy( &nextNext, &buffer[sizeof(int) * 0], sizeof(int) );

    if(nextNext == bloqueeOcupado)
    {
      memcpy(&buffer[sizeof(int) * 0], &bloqueeLibrador, sizeof(int) );
      writeFileCaller(next, buffer, fileName);
      finDeBloquees = bloqueeLibrador;
      tamanoLibre++;

      readFileCaller(1, buffer, fileName);
      memcpy(&buffer[sizeof(int) * 1], &tamanoLibre, sizeof(int) );
      memcpy(&buffer[sizeof(int) * 3], &bloqueeLibrador, sizeof(int));
      writeFileCaller(1, buffer, fileName);

    }

    viajero = next;



  }










}
