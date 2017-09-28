//Shortest-time-remaining= Formula para calcular tiempo de espera de cada proceso, si un proceso hace dos esperas separadas
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
  int id;
  char letra;
  char *frase;
  int * cedula;
} Persona;

void * PrintPersona(void *params);

int main(int argc, char *args[])
{
  int num_philosophers = 5;
  pthread_t threads[num_philosophers];
  int nums[5];
  nums[0] = 0;
  nums[1] = 1;
  nums[2] = 2;
  nums[3] = 3;

  int nums2[5];
  nums2[0] = 40;
  nums2[1] = 10;
  nums2[2] = 20;
  nums2[3] = 30;

  char * frase[30];
  frase[0] = "javi";
  frase[1] = "noel";
  frase[2] = "sabi";
  frase[3] = "merino";
  frase[4] = "jalluzaj";



  int i;
  for(i = 0; i< num_philosophers; i++)
  {
    Persona *amigo = malloc(sizeof(Persona));
    amigo->id = i;
    amigo->letra = i + '0';
    amigo->frase = frase[i];
    amigo->cedula = (i % 2 == 0)? nums : nums2;

    pthread_create(&threads[i], NULL, PrintPersona , (void *)amigo );

  }

  pthread_exit(NULL);
}

void * PrintPersona(void *params)
{
    Persona person = *(Persona *)params;
    printf("Yo %d letra: %c frase: %s soy %d \n", person.id, person.letra, person.frase, person.cedula[0]);
}
