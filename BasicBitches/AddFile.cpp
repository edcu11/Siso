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

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif


typedef int (*writeMemory)(int , char*, char *);
typedef int (*readMemory)(int , char*, char *);
typedef int (*writeFile)(int , char*, char * );
typedef int (*readFile)(int , char*, char *);

class FreeIndexData{

  public:
    FreeIndexData()
    {
      buffer = (char*)calloc(4096,1);
    }

    char * buffer;
    int index;
    int block;
};


std::bitset<8> PrintBitSet(char * buffer, int pos)
{

  std::bitset<8> bits(buffer[pos]);
//  std::cout <<"in func"<< bits << "\n" ;
  return bits;
}



int fileSize(const char *add){
    std::ifstream mySource;
    mySource.open(add, std::ios_base::binary);
    mySource.seekg(0, std::ios_base::end);
    int size = mySource.tellg();
    mySource.close();
    return size;
}

int GetPostToWriteFileName(char* buffer){


  char * nombreDeArchivo = (char *)calloc(20,1);
  int direccion = 0;
  int tamano = 0;
  for (size_t i = 0; i < BLOCK_SIZE; i += 20 + (sizeof(int) * 2)) {
    memcpy(nombreDeArchivo, buffer + i, 20);
    memcpy(&direccion, buffer + i + 20, 4);
    memcpy(&tamano, buffer + i + 24, 4);
    printf("file--%s  %d %d\n", nombreDeArchivo, direccion, tamano);
    if(nombreDeArchivo[0] == 0)
    {
      return i;
      break;
    }
  }
}

FreeIndexData GetFreeFileBlock(char * fileName,  int (*read)(int , char*, char *) )
{

  FreeIndexData freeIndexData ;

  freeIndexData.buffer = (char *)calloc(BLOCK_SIZE,1);
  read(1, freeIndexData.buffer, fileName);

  int bitmapBlocks;
  long int diskSize;
  memcpy(&bitmapBlocks, &freeIndexData.buffer[0], 4);
  for (size_t i = 0; i < bitmapBlocks; i++)
  {
    read(i + 2, freeIndexData.buffer, fileName);
    for (size_t bitsetCount = 0; bitsetCount < BLOCK_SIZE; bitsetCount++)
    {
      std::bitset<8> bits = PrintBitSet(freeIndexData.buffer, bitsetCount);
      for (size_t bitsChecked = 0; bitsChecked < 8; bitsChecked++)
      {
        if(bits[bitsChecked] == 0)
        {
          freeIndexData.buffer[bitsetCount] = freeIndexData.buffer[bitsetCount] | (1 << bitsChecked);
          freeIndexData.block = i;
          freeIndexData.index =  bitsChecked + (bitsetCount * 8);
          return freeIndexData;
        }
      }
    }
  }


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


int main(int argc, char const *argv[])
{

  void *  sharedObj = GetBasicFileSystemObject();

  if(argc < 4)
  {
    printf("Insuficient parameters: %d \n", argc);
    return 0;
  }

  writeFile writeFileCaller = (writeFile) dlsym(sharedObj, "writeFile");
  readFile readFileCaller = (writeFile) dlsym(sharedObj, "readFile");


  if(strcmp(argv[1], "-m") == 0)
  {

    writeMemory writeMemoryCaller = (writeMemory) dlsym(sharedObj, "writeSharedMemory");
    readMemory readMemoryCaller = (readMemory) dlsym(sharedObj, "readSharedMemory");

  }

  if(strcmp(argv[1], "-f") == 0)
  {

    char *buffer = (char *)calloc(BLOCK_SIZE, 1);
    FreeIndexData freeIndexData;
    freeIndexData = GetFreeFileBlock((char *)argv[2], readFileCaller);
    writeFileCaller(2 + freeIndexData.block, freeIndexData.buffer, (char*)argv[2]);
    //printf("Wrote back bitmap at: %d\n", freeIndexData.block + 2 );

    readFileCaller(1, buffer, (char*)argv[2]);
    int bitmapBlocks;
    memcpy(&bitmapBlocks, &buffer[0], 4);
    printf("%d\n", sizeof(int));

    readFileCaller(bitmapBlocks + 2, buffer, (char*)argv[2]);
    int posToWrite = GetPostToWriteFileName(buffer);
    int nameLength = strlen(argv[3]);

    memcpy(buffer + posToWrite, (char*)argv[3], nameLength);
    memcpy(&buffer[posToWrite + 20], &freeIndexData.index, sizeof(int) );
    int neededBlocks = (fileSize(argv[3]) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    memcpy(&buffer[posToWrite + 24], &neededBlocks, sizeof(int));
    writeFileCaller(bitmapBlocks + 2, buffer, (char *)argv[2]);
  //  printf("Writing First page at %d\n", freeIndexData.index );

    int firstPageBlockAmount= neededBlocks / 1024;
    int blocksWritten = 0;
    int firstPageNextIndex = 0;
    int firstPageBlockAddress = freeIndexData.index;
    int sndPageBlockAddress = 0;
    int sndPageNextIndex = 0;

    for (size_t blocksWritten = 0; blocksWritten < neededBlocks; ) {
    //  printf("start 1 for\n");
      buffer = (char *)calloc(4096, 1);
      freeIndexData = GetFreeFileBlock((char *)argv[2], readFileCaller);
      writeFileCaller(2 + freeIndexData.block, freeIndexData.buffer, (char*)argv[2]);
      readFileCaller(firstPageBlockAddress, buffer, (char*)argv[2]);
      memcpy(&buffer[firstPageNextIndex], &freeIndexData.index, sizeof(int) );
      firstPageNextIndex += sizeof(int);
      writeFileCaller(firstPageBlockAddress, buffer, (char*)argv[2]);
      sndPageBlockAddress = freeIndexData.index;
      sndPageNextIndex = 0;
  //    printf("Writing Second page at %d\n", freeIndexData.index );

      for (size_t sndPageBlocksCopied = 0; sndPageBlocksCopied < 1024; sndPageBlocksCopied++) {

        if(blocksWritten > neededBlocks)
        {
          printf("done writing!\n");
          break;
        }

        buffer = (char*)calloc(4096,1);
        freeIndexData = GetFreeFileBlock((char *)argv[2], readFileCaller);
        writeFileCaller(2 + freeIndexData.block, freeIndexData.buffer, (char*)argv[2]);
  //      printf("Writing sndPageBlocksCopied: %d  at %d\n", sndPageBlocksCopied, freeIndexData.index );

        readFileCaller(sndPageBlockAddress, buffer, (char*)argv[2]);
        memcpy(&buffer[sndPageNextIndex], &freeIndexData.index, sizeof(int) );
        sndPageNextIndex += sizeof(int);
        writeFileCaller(sndPageBlockAddress, buffer, (char*)argv[2]);


        //empiezo a pasar data
        readFileCaller(blocksWritten++, buffer,  (char*)argv[3]);

        writeFileCaller(freeIndexData.index, buffer, (char*)argv[2]);
//        printf("wrote block %d at %d\n", blocksWritten, freeIndexData.index);

      }


    }


  }



  dlclose(sharedObj);

  return 0;
}
