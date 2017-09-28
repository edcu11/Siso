#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

static int cantObjetos = 0;
static int cantDominios =  0;


struct Acceso
{
  std::string acceso;
  std::string dominioNombre;

};


Acceso crearAcceso(std::string acceso, std::string dominioNombre)
{
  Acceso a;
  a.acceso = acceso;
  a.dominioNombre = dominioNombre;
  return a;
}



struct Dominio
{
  Acceso accesos[100];
  std::string nombre;
  int contador;

};



struct Objeto
{
  std::string nombre;
};

Dominio CrearDominio(std::string nombre)
{
  Dominio d;
  d.nombre = nombre;
  d.contador = 0;
}


Objeto CrearObjeto(std::string nombre)
{
  Objeto o;
  o.nombre = nombre;
}



void AgregarAcceso(Acceso acceso, Dominio dominio)
{

  dominio.accesos[dominio.contador] = acceso;
  dominio.contador++;

}



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


void encrypt(const char * M, const char * K, char * c)
{
  int tamano = strlen(M);
  int keyLength = strlen(K);

  printf("\ntamano %d\n", tamano);
  printf("key le %d\n", keyLength );

  for (size_t i = 0; i < tamano; i++) {

    c[i] = M[i] - K[(i) % keyLength];

  }

}

void decrypt(const char * M, const char * K, char * c)
{
  int tamano = strlen(M);
  int keyLength = strlen(K);

  for (size_t i = 0; i < tamano; i++) {

    c[i] = (M[i] + K[(i) % keyLength]);

  }

}

void encryptMessage()
{

  const char * mensaje;
  char  mensajeEncryptado[100];
  char  mensajeDesencryptado[100];
  const char *  key = "Llave";
  std::string in;

  printf("Ingrese mensaje: ");
  std::cin >> in;
  mensaje = in.c_str();

  encrypt(mensaje, key, mensajeEncryptado);
  printf("Mensaje encryptado: %s\n", mensajeEncryptado);

  decrypt(mensajeEncryptado, key, mensajeDesencryptado);
  printf("Mensaje des: %s\n", mensajeDesencryptado);

}



int posDeDominio(Dominio dominios[], std::string buscado)
{

  for (size_t i = 0; i < cantDominios; i++) {
    if(dominios[i].nombre.compare(buscado) == 0)
    {
      return i;
    }
  }

  return -1;

}

int posDeDominio(Objeto objetos[], std::string buscado)
{

  for (size_t i = 0; i < cantObjetos; i++) {
    if(objetos[i].nombre.compare(buscado) == 0)
    {
      return i;
    }
  }

  return -1;

}

/*
void Matriz(Objeto objetos[], Dominio dominios[])
{

  std::string in;
  Dominio d;
  Objeto o;

  printf("1. Dominio 2. Objeto 3. Acceso\n");

  std::cin >> in;

  if(in.compare("1 a") == 0)
  {
    printf("Ingrese nombre de dominio:");
    cin >> in;
    dominios[cantDominios] = CrearDominio(in);
    cantDominios++;
  }

  if(in.compare("1 e") == 0)
  {
    printf("Ingrese nombre de dominio:");
    cin >> in;

    for (size_t i = 0; i < cantDominios; i++) {
      if(dominios[i].nombre.compare(in) == 0)
      {
        printf("Borrado con exito\n");
        dominios[i] = dominios[cantDominios - 1];
        cantDominios--;
      }
    }

  }

  if(in.compare("2 a") == 0)
  {
    printf("Ingrese nombre de Objeto:");
    cin >> in;
    dominios[cantObjetos] = CrearObjeto(in);
    cantObjetos++;
  }

  if(in.compare("2 e") == 0)
  {
    printf("Ingrese nombre de objeto a eliminar\n");
    cin >> in;

    for (size_t i = 0; i < cantObjetos; i++) {
      if(objetos[i].nombre.compare(in) == 0)
      {
        printf("Borrado con exito\n");
        objetos[i] = objetos[cantDominios - 1];
        cantObjetos--;
      }
    }

  }


  if(in.compare("3 a") == 0)
  {
    printf("Ingrese nombre de objeto: " );
    cin >> in;

    int ObjetoPos = posDeObjeto(objetos, in);
    if(ObjetoPos == -1)
    {
      printf("Objet no existe\n");
    }

    printf("Ingrese nombre de Dominio: " );
    cin >> in;
    int dominoPos = posDeDominio(dominios, in);

    if(dominoPos == -1)
    {
      printf("Dominio no existe\n");
    }

    printf("Ingrese nombre de Acceso: " );
    cin >> in;

    Acceso a;
    a.nombre = in;
    a.dominio = dominios[dominoPos];
    AgregarAcceso(a, bjetos[ObjetoPos]);
  }





}

*/

int main(int argc, char **argv[])
{

  int requestQueue[100];
  int size = 0;
  int headStart = 0;
  int tamano = 0;
  std::string menu = " ";
  Objeto objetos[100];
  Dominio dominios[100];

  while (menu.compare("exit") != 0) {
    printf("1.Encriptacion  2.Matriz 3. Disks \n ");


    if(menu.compare("encrypt") == 0)
    {
      encryptMessage();
    }

    if(menu.compare("matriz") == 0)
    {
    //  Matriz(objetos, dominios);
    }

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
