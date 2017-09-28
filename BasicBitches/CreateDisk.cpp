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

#ifndef BasicFileSystemPath
#define BasicFileSystemPath "/vagrant/Siso/BasicBitches/BasicFileSystem.so"
#endif


int GetSizeMultiplier(char ByteSize)
{

    switch (ByteSize) {
      case 'K':
        return 1024;
      case 'M':
        return 1024 * 1024;
      case 'G':
        return 1024 * 1024 * 1024;
    }

}

char * InitBitMap(int cantidadDeBloquees, char * buffer)
{
  int uno = 1;
  int byte = 0;
  for (size_t i = 0; i < (cantidadDeBloquees + 2); i++) {
    byte = floor(i/8.0f);
    buffer[byte] = buffer[byte] | (uno << (i%8) );
  }
}

long int GetDiskSize(const char * tamano)
{

  std::string numberOnly = tamano;
  int tamanoDeString = numberOnly.size();// strlen(tamano);
  int Multiplier = GetSizeMultiplier(tamano[tamanoDeString - 2]);
  numberOnly.resize(tamanoDeString - 2);
  return (long int)std::stoi(numberOnly.c_str()) * Multiplier;

}

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


typedef int (*writeMemory)(int , char*, char *);
typedef int (*readMemory)(int , char*, char *);
typedef int (*writeFile)(int , char*, char * );


int main(int argc, char const *argv[])
{

  void *  sharedObj = GetBasicFileSystemObject();

  if(argc < 4)
  {
    printf("Insuficient parameters: %d \n", argc);
    return 0;
  }

  long int diskSize = GetDiskSize(argv[3]);
  float blockNumber = diskSize / 4096.0f;
  float BitmapBits = blockNumber / 8.0f;
  int BitmapBlocks = ceil(BitmapBits / 4096.0f);

  if(strcmp(argv[1], "-m") == 0)
  {
    int shared_mem_key = 5555; ///ftok(argv[2], proj_id);
    int shm_id = shmget(shared_mem_key, diskSize , IPC_CREAT | S_IRUSR | S_IWUSR);
    char * shmaddr = (char *) shmat(shm_id, NULL, 0);

    writeMemory writeMemoryCaller = (writeMemory) dlsym(sharedObj, "writeSharedMemory");
    readMemory readMemoryCaller = (readMemory) dlsym(sharedObj, "readSharedMemory");


    std::string primerBloque = std::to_string(BitmapBlocks + 2) +"\n"+
                               //"\nFreeSpace: " + std::to_string((int)blockNumber - (int)BitmapBlocks - 2);
                               std::to_string(diskSize) + "\n" + argv[2];

    writeMemoryCaller(1, (char*)primerBloque.c_str(), shmaddr);
    char * buffer = (char *) malloc(4096);
    readMemoryCaller(1, buffer, shmaddr);
    char * bitMapBuffer = (char *) calloc(4096  * (BitmapBlocks + 2), 1 );
    InitBitMap(BitmapBlocks, bitMapBuffer);

   for (size_t blocksCopied = 0; blocksCopied < BitmapBlocks; blocksCopied++) {
     memmove(buffer, bitMapBuffer + (blocksCopied *  4096), 4096);
     writeMemoryCaller(2 + blocksCopied, buffer, shmaddr);
    }

    /*
    readMemoryCaller(2, buffer, shmaddr);
    std::bitset<8> x(buffer[2]);
    std::cout << x;
    readMemoryCaller(1, buffer, shmaddr);
    printf("lo que %s\n", buffer);
    */

  }

  if(strcmp(argv[1], "-f") == 0)
  {
    writeFile writeFileCaller = (writeFile) dlsym(sharedObj, "writeFile");

    std::ofstream ofs (argv[2], std::ofstream::out);

    char* fbuffer = (char *)calloc(4096,1);
    ofs.seekp(diskSize, std::ios::beg);
    ofs.write("",1);
    ofs.close();


    memcpy(&fbuffer[sizeof(int) * 0], &BitmapBlocks, sizeof(int) );
    memcpy(&fbuffer[sizeof(int) * 1], &diskSize, sizeof(diskSize) );

    char * bitMapBuffer = (char *) calloc(4096  * (BitmapBlocks + 2), 1 );
    writeFileCaller(1, fbuffer, (char *)argv[2]);
    InitBitMap(BitmapBlocks + 1, bitMapBuffer);

    for (size_t blocksCopied = 0; blocksCopied < BitmapBlocks; blocksCopied++) {
      memmove(fbuffer, bitMapBuffer + (blocksCopied *  4096), 4096);
      writeFileCaller(2 + blocksCopied, fbuffer, (char*)argv[2]);
     }


  }




  dlclose(sharedObj);

  return 0;
}
