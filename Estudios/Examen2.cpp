#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <list>


class SegmentData
{
public:
  int base;
  int limit;
};

class PageAddres
{
public:
  int * physicaAddres;
  PageAddres()
  {
    physicaAddres = {0};
  }

};

class PageTable
{
public:
  PageAddres *pages;
  PageTable()
  {
    pages = new PageAddres();
  }
};

class OuterPageTable
{
public:
  PageTable * PageTables;
  OuterPageTable()
  {
    PageTables = new PageTables();
  }
};


class MMU
{
  public:
  virtual int getRealAddres(unsigned int logicalAddress) = 0;
};

class AsignacionContinua : public MMU
{
public:
  int base;
  int limit;


  AsignacionContinua (int base, int limit)
  {
    this->base = base;
    this->limit = limit;
  }

  int getRealAddres(unsigned int logicalAddress)
  {
    return base + logicalAddress;
  }
};

class Segmentacion : public MMU
{
public:
  SegmentData* segmentTable;


  Segmentacion(SegmentData* segmentTable)
  {
    this->segmentTable = segmentTable;
  }

  int getRealAddres(unsigned int logicalAddress)
  {
    //PARA DIRECCINOAR 40 SEGMENTOS OCUPO LOG BASE 2 (40)  = 5.4 = 6 BITS
    unsigned int offset = logicalAddress & (0x000000FF);
    //PARA DIRECCIONAR LAS 50 "BYTES" QUE HAY EN CADA SEGMENTO OCUPO TAMBIEN 6
    unsigned int index = (logicalAddress & (0x00000F00) ) >> 8;
    printf("Index= %d %X, offset= %d %X\n", index, index, offset, offset);

    printf("%d\n", segmentTable[5].limit);
    if( offset > segmentTable[index].limit)
    {
      printf("Offset es demasiado grande\n");
      return -1;
    }

    return segmentTable[index].base + offset;

  }
};

class Paginacion : public MMU
{
public:
  OuterPageTable * pageTable;

  Paginacion ( OuterPageTable * pageTable )
  {
    this->pageTable = pageTable;
  }

  int getRealAddres(unsigned int logicalAddress)
  {
    int p1 = (logicalAddress >> 22) & 1023;
    int p2 = (logicalAddress >> 12) & 1023;
    int d = (logicalAddress & (0x00000FFF));
    return 2;
  }
};

void InitSegmentTable(SegmentData seg[])
{
  for (int i = 0; i < 16; i++) {
    seg[i].base = i * 256;
    seg[i].limit = 256;
  }
}

void InitPageTbale(OuterPageTable pt)
{

  pt.tables[40].addresses[15].physicaAddres[5] = 20;
}

int main(int argc, char const *argv[]) {


  srand( (unsigned) time(NULL));

  MMU *mmu = new AsignacionContinua(400, 200);


  printf("Asignacion continua: %d\n", mmu->getRealAddres(2));

  SegmentData seg[40];
  InitSegmentTable(seg);
  Segmentacion *mmuS = new Segmentacion(seg);
  printf("%d %d \n", mmuS->segmentTable[20].base, mmuS->segmentTable[20].limit  );

  printf("Segmentacion = %d\n", mmuS->getRealAddres(400) );

  InitPageTbale(OuterPageTable pt);
  Paginacion *mmuP = new Paginacion();



/*  printf("Asignacion continua: %d\n", apunt->getRealAddres(2));
  printf("Asignacion continua: %d\n", apunt->getRealAddres(2));*/


  return 0;
}
