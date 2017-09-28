#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

sem_t mutex, full, empty;
int *buffer;
int arrayPos = 0;

void* producer()
{
  int randomNumber;
  while(1)
  {
    randomNumber = rand() % 1000;
    sem_wait(&empty);
    sem_getvalue(&empty, &arrayPos);
    sem_wait(&mutex);
    arrayPos++;
    printf("Produced: %d At: %d\n", randomNumber, arrayPos);
    buffer[arrayPos] = randomNumber;
    sem_post(&mutex);
    sem_post(&full);
  }
}

void* consumer(void * id)
{
  int tid = *(int *)id;
  int numeroConsumido;
  while(1)
  {
    sem_wait(&full);
    sem_getvalue(&full, &arrayPos);
    sem_wait(&mutex);
    arrayPos = arrayPos + 1;
    numeroConsumido = buffer[arrayPos];
    sem_post(&mutex);
    sem_post(&empty);
    printf("Consumed: %d, At: %d By: %d\n", numeroConsumido, arrayPos, tid);
  }
}

void StartThread(pthread_t tid, pthread_attr_t attr, int *id)
{
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, consumer, id);
}

int main(int argc, char *argv[])
{
  int cant = 100;
  int counter;
  pthread_t tid[5];
  pthread_attr_t attr;
  int id[5];
  id[1] = 1;
  id[2] = 2;
  id[3] = 3;
  id[4] = 4;

  buffer = malloc(sizeof(int) * (cant + 1));
  for(counter = 0; counter < 100; counter++)
  {
    buffer[counter] = 0;
  }
  srand( (unsigned) time(NULL));

  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, cant);


  pthread_attr_init(&attr);
  pthread_create(&tid[0], &attr, producer, NULL);

  for (counter = 1; counter < 5; counter++)
  {
    StartThread(tid[counter], attr, &id[counter]);
  }

  for (counter = 0; counter < 5; counter++)
  {
    pthread_join(tid[counter], NULL);
  }

}
