#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>


int ObtenerDiferencia(int numeroInicial, int numeroFinal)
{

  int retorno = numeroInicial - numeroFinal;
  return (retorno < 0)? (retorno * -1) : retorno;

}


int GoTo(int * posicionInicial, int posicionObjetivo)
{

  int dif =  ObtenerDiferencia(*posicionInicial, posicionObjetivo);
  *posicionInicial = posicionObjetivo;
  return dif;

}


int GetNextRequestPosSSF(int headPos, int requestQueue[], int size)
{
  int menorDiferencia = 999;
  int menorPos = 0;
  int diferencia = 0;
  int menor = 0;

  for (size_t i = 0; i < size; i++)
  {
    diferencia = ObtenerDiferencia(headPos, requestQueue[i]);
    if(diferencia < menorDiferencia && requestQueue[i] != -1)
    {
      menorDiferencia = diferencia;
      menorPos = i;
    }

  }
  return menorPos;
}

int DiskSSF(int headStart, int requestQueue[], int size)
{

    int recorrido = 0;
    int nextRequest = -1;
    int nextRequestPos = 0;


    for (size_t request = 0; request < size; request++)
    {
      nextRequestPos = GetNextRequestPosSSF(headStart, requestQueue, size);
      nextRequest = requestQueue[nextRequestPos];

      recorrido += GoTo(&headStart, nextRequest);
      requestQueue[nextRequestPos] = -1;
      printf("recorrido %d --- head: %d\n", recorrido, headStart );

    }

    return recorrido;
}


int GetCloserRequestPos(int headPos, int requestQueue[], int size, int orientacion)
{
  int masCercano = 999;
  int orientacionCorrecta = 0;
  int masCercanoPos = 0;
  int diferencia = 0;
  int menor = 0;

  for (size_t i = 0; i < size; i++)
  {
    diferencia = ObtenerDiferencia(headPos, requestQueue[i]);
    orientacionCorrecta = (orientacion == 1)? requestQueue[i] > headPos : requestQueue[i] < headPos;

    if(diferencia < masCercano && requestQueue[i] != -1 && orientacionCorrecta)
    {
      masCercano = diferencia;
      masCercanoPos = i;
    }

  }
  return masCercanoPos;
}

int HayRequestHasta(int headPos, int limite, int requestQueue[], int size, int orientacion)
{
  int hayRequest = 0;
  int requestValida = 0;

  for (size_t request = 0; request < size; request++) {
    hayRequest = (orientacion) ? requestQueue[request] < limite : requestQueue[request] > limite;
    requestValida = (orientacion) ? requestQueue[request] > headPos : requestQueue[request] < headPos;

    if(hayRequest && requestValida)
    {
      return 1;
    }

  }
  return 0;
}


int DiskAll(int headStart, int requestQueue[], int size, int metodo, int limite)
{

  int recorrido = 0;
  int nextRequest = -1;
  int nextRequestPos = 0;
  int orientacion = 1;
  int limiteLook = 0;


  for (size_t request = 0; request < size; request++)
  {
    nextRequestPos = GetCloserRequestPos(headStart, requestQueue, size, orientacion);
    nextRequest = requestQueue[nextRequestPos];

    recorrido += GoTo(&headStart, nextRequest);
    requestQueue[nextRequestPos] = -1;
    printf("recorrido %d --- head: %d\n", recorrido, headStart );


    if(!HayRequestHasta(headStart, limite, requestQueue, size, orientacion) && request < size -1)
    {
      //es cscan
      if(metodo == 0)
      {
        printf("------Reiniciando cabezal a Inicio------" );
        recorrido += GoTo(&headStart, limite);
        recorrido += GoTo(&headStart, 0);
      }

      //es clook
      if(metodo == 1)
      {
        printf("--------Cambiando orientacion-----\n");
        orientacion = (orientacion)? 0: 1;
      }

      printf("------Recorrido: %d head: %d------", recorrido , headStart);

    }

  }

  return recorrido;
}



void CopiarArreglo(int arr1[], int arr2[],int size )
{


  for (size_t i = 0; i < size; i++)
  {
    arr1[i] = arr2[i];
  }

}




void PrintDisksSchedule(int requestQueue[], int headStart, int size, int limite)
{
  int recorrido =  0;
  int copyQueue[100];//{98, 183, 37, 122, 14, 124, 65, 67};

  CopiarArreglo(copyQueue, requestQueue, size);

  recorrido = DiskSSF(headStart, requestQueue, size);
  printf("Recorrido con SSF: %d\n \n\n\n", recorrido);

  CopiarArreglo(requestQueue, copyQueue, size);

  recorrido = DiskAll(headStart, requestQueue, size, 0, limite);
  printf("Recorrido con CSCAN: %d\n\n\n", recorrido);

  CopiarArreglo(requestQueue, copyQueue, size);

  recorrido = DiskAll(headStart, requestQueue, size, 1, limite);
  printf("Recorrido con SCAN: %d\n\n\n", recorrido);

}


void LlenarArreglo(int request[], int size)
{
  printf("Ingrese numeros: ");
  for (size_t i = 0; i < size; i++) {
    std::cin >> request[i];
  }

  for (size_t i = 0; i < size; i++) {
    printf("%d\n", request[i]);
  }

}

int main(int argc, char **argv[])
{

  int requestQueue[100];
  int size = 0;
  int headStart = 0;
  int tamano = 0;
  std::string menu = " ";

  while (menu.compare("exit") != 0) {
    printf("1.Encriptacion  2.Matriz 3. Disks \n ");

    if(menu.compare("disk") == 0)
    {
      printf("Cuantos numeros: \n");
      std::cin >> size;
      printf("Ingrese donde inicia el cabezal:" );
      std::cin >> headStart;
      printf("Ingrese tamano:");
      std::cin >> tamano;
      LlenarArreglo(requestQueue, size);
      PrintDisksSchedule(requestQueue, headStart, size, tamano);
    }

    std::cin >> menu;

  }




/*
*/


	return 0;
}
