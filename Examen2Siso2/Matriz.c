#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char ObtenerByteDecriptado(int arrayNumber, char keyValue)
{
  return arrayNumber ^ keyValue;
}



void ShowDecryptedMessage(int array[], int size, char * llave)
{

  char decryptedByte = 0;
  char * numero;
  int keyLength = strlen(llave);
  numero = (char *)array;

  for (size_t arrayPos = 0; arrayPos < size *  sizeof(int) ; arrayPos++)
  {
    decryptedByte = ObtenerByteDecriptado(numero[arrayPos], llave[(arrayPos) % keyLength]);
    printf("%c\n", decryptedByte);
  }

}


int main(int argc, char **argv[])
{

  char * llave;

  llave = "llave";

  int array[] =  { 37817621, 2051088,1296891924 };

  ShowDecryptedMessage(array, 3, llave);



//  printf("%c\n", llave[(i) % key_length]);




	return 0;
}
