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

  if(argc < 3)
  {
    printf("Insuficient parameters: %d \n", argc);
    return 0;
  }

  writeFile writeFileCaller = (writeFile) dlsym(sharedObj, "writeFile");
  readFile readFileCaller = (writeFile) dlsym(sharedObj, "readFile");

  int cantidadDeBloquees = atoi(argv[2]);

  std::ofstream ofs (argv[1], std::ofstream::out);


  char* buffer = (char *)calloc(512,1);
  ofs.seekp(cantidadDeBloquees * 512, std::ios::beg);
  ofs.write("",1);
  ofs.close();

  int tamanoTotal = cantidadDeBloquees;
  int tamanoLibre = cantidadDeBloquees;
  int inicioDeBloquees = 2;
  int nextBlock = 0;


  memcpy(&buffer[sizeof(int) * 0], &tamanoTotal, sizeof(int) );
  memcpy(&buffer[sizeof(int) * 1], &tamanoLibre, sizeof(int) );
  memcpy(&buffer[sizeof(int) * 2], &inicioDeBloquees, sizeof(int));
  memcpy(&buffer[sizeof(int) * 3], &cantidadDeBloquees, sizeof(int));




  writeFileCaller(1, buffer, (char*)argv[1]);

  for (size_t blocksFormatted = 2; blocksFormatted < cantidadDeBloquees; blocksFormatted++) {
    buffer = (char*)calloc(512,1);
    nextBlock = blocksFormatted + 1;

    if(nextBlock == cantidadDeBloquees)
    {
      nextBlock = -1;
    }

    memcpy(&buffer[0], &nextBlock, 4);
    writeFileCaller(blocksFormatted, buffer, (char*)argv[1]);
    printf("wrote index %d \n");
  }











}
