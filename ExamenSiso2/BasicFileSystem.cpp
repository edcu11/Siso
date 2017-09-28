#include "stdio.h"
#include <string.h>
#include <iostream>
#include <fstream>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 512
#endif

extern "C" int writeSharedMemory(int address, char * buffer, char * shmaddr)
{
  strncpy(&shmaddr[address * BLOCK_SIZE], buffer, BLOCK_SIZE);
  return 0;
}


extern "C" int readSharedMemory(int address, char * buffer, char * shmaddr)
{

  strncpy(buffer, &shmaddr[address * BLOCK_SIZE], BLOCK_SIZE);
  return 0;
}

extern "C" int writeFile(int address, char * buffer, char * fileName)
{
  std::fstream fs;
  fs.open(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
  fs.seekp(address * BLOCK_SIZE, std::ios::beg);
  fs.write(buffer,BLOCK_SIZE);
  fs.close();

  return 0;
}

extern "C" int readFile(int address, char * buffer, char * fileName)
{

  std::fstream fs;
  fs.open(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
  fs.seekp(address * BLOCK_SIZE, std::ios::beg);
  fs.read(buffer, 512);
  fs.close();




}
