//gcc 5.4.0

#include  <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NTHREADS 8

void * sort(void *param);
int array[64];

void PutValues(char * argv[], int argc)
{
    int cont;
    for(cont = 0; cont < argc -1 ; cont++)
    {
        array[cont] = atoi(argv[cont + 1]);
    }

}

void PrintValues(int argc)
{
    int cont;
    for(cont = 0; cont < argc -1 ; cont++)
    {
        printf("Num: %d \n", array[cont]);
    }
}

int main(int argc, char * argv[])
{
    printf(" %d \n", argc - 1);

    pthread_t tid[NTHREADS];
    int iret1 = 0;
    //int palam[1];

    //void *retval;
    //int * subarray;

    PutValues(argv, argc);
   // PrintValues(argv, argc);


    int cont;
    int tamanoParticion = (argc -1) / 8;

    int inicio = 0 - tamanoParticion; //donde empezara a revisar el arreglo
    printf("tamanoParticion: %d\n", tamanoParticion);
    int palam[2];

    for( cont = 0; cont < NTHREADS; cont++)
    {
        //palam = malloc(sizeof(int) * 2);
        palam[0] = malloc(sizeof(int));
        palam[1] = malloc(sizeof(int));
        palam[0] = inicio;
        palam[1] = tamanoParticion;
        printf("%d %d \n", palam[0], palam[1] );
        pthread_create(&tid[cont], NULL, sort, &palam);
        inicio += tamanoParticion;
        if(iret1)
        {
             fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
             return 0;
         }

    }

    for(cont = 0; cont < NTHREADS; cont++)
    {
        pthread_join( tid[cont], NULL);
        //subarray = (int *)retval;
    }


    PrintValues(argc);
    return 0;
}

void * sort(void * param)
{
    int *arreglo = (int*)param;
    printf("\n");
    int inicio = arreglo[0];
    int fin = inicio + arreglo[1];
    printf("Thread: %d  Size: %d     ", inicio, fin);


    int cont;
    for(cont = inicio; cont < fin; cont ++)
    {
        printf(" %d,", array[cont] );
    }

    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = inicio; i < fin - 1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < fin; j++)
        {
          if (array[j] < array[min_idx])
          {
            min_idx = j;
          }
        }

        // Swap the found minimum element with the first element
        int temp = array[i];
        array[i] = array[min_idx];
        array[min_idx] = temp;

    }
    printf(" Ordered: ");

    for(cont = inicio; cont < fin; cont ++)
    {
        printf(" %d,", array[cont] );
    }




    return 0;
}
