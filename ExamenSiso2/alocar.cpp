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


  int inicioDeBloquees = 0;
  int finDeBloquees = 0;
  int nuevoInicio = 0;
  int bloqueeOcupado = -1;
  int tamanoLibre = 0;

  readFileCaller(1, bufferInicial, fileName );
  memcpy( &tamanoLibre, &bufferInicial[sizeof(int) * 1], sizeof(int) );
  memcpy( &inicioDeBloquees, &bufferInicial[sizeof(int) * 2], sizeof(int) );
  memcpy( &finDeBloquees, &bufferInicial[sizeof(int) * 3], sizeof(int) );



  //vamos al bloquee que queremos alocar y cargamos apra ver cual sera el nuevo primero
  readFileCaller(inicioDeBloquees, buffer, fileName);
  memcpy(&nuevoInicio, &buffer[0], sizeof(int));

  if(nuevoInicio == -1)
  {
    printf("No hay bloquees libres\n");
    return 0;
  }

  //escribimos nuevo inicio
  memcpy(&bufferInicial[sizeof(int) * 2], &nuevoInicio, sizeof(int));
  tamanoLibre--;
  memcpy(&bufferInicial[sizeof(int) * 1], &tamanoLibre, sizeof(int));
  writeFileCaller(1, bufferInicial, fileName);

  memcpy(&buffer[0], &bloqueeOcupado, sizeof(int));
  writeFileCaller(inicioDeBloquees, buffer, fileName);









}
