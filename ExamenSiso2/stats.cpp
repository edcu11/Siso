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

  char* buffer = (char *)calloc(512,1);


  int tamanoTotal = 0;
  int tamanoLibre = 0;
  int inicioDeBloquees = 0;
  int cantidadDeBloquees = 0;

  readFileCaller(1, buffer, (char*)argv[1]);

  memcpy( &tamanoTotal, &buffer[sizeof(int) * 0], sizeof(int) );
  memcpy( &tamanoLibre, &buffer[sizeof(int) * 1], sizeof(int) );

  printf("Disk: %s \ntotal:%d \nFree: %d \n",(char*)argv[1], tamanoTotal, tamanoLibre);





  }
