#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define NUM_PHILOSOPHERS 13


int id[5];

sem_t lock;
sem_t palillos[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];

char * hola = "variable a leer, leida GG M8";

void think(int position);
void eat(int position);

int IsEven(int id)
{
   if(id % 2 == 0)
   {
       return 1;
   }
   return 0;
}

void pickEvenForks(int philosopherId)
{

    sem_wait(&palillos[(philosopherId + 1) % NUM_PHILOSOPHERS]);
    printf("%d Am Even! Got first thingy %d  ---------------", philosopherId, (philosopherId + 1) % NUM_PHILOSOPHERS);
    sem_wait(&palillos[philosopherId]);
    printf("|||-----------%d Got second thingy %d ", philosopherId, philosopherId);

    return;
}

void pickOddForks(int philosopherId)
{

    sem_wait(&palillos[philosopherId]);
    printf("%d Am Odd! Got first thingy %d  ---------------", philosopherId, philosopherId);
    sem_wait(&palillos[(philosopherId + 1) % NUM_PHILOSOPHERS]);
    printf("|||-----------%d Got second thingy %d ", philosopherId, (philosopherId + 1) % NUM_PHILOSOPHERS);
    return;
}


void InitVars(sem_t *lock, sem_t *palillos)
{
  int i;
  for(i = 0; i < NUM_PHILOSOPHERS; i++) {
    sem_init(&palillos[i], 0, 1);
    id[i] = i;
  }

  sem_init(lock, 0, NUM_PHILOSOPHERS - 1);
}


void *philosopher(void *params)
{
  int i;
  int philosopherId = *(int *)params;
  printf("Philosopher #%d started!", philosopherId);

  for(i = 0; i < 10; ) {
    think(philosopherId);

    sem_wait(&lock);
    if(IsEven(philosopherId))
    {
        pickEvenForks(philosopherId);
    }
    else
    {
        pickOddForks(philosopherId);
    }
    eat(philosopherId);
    sem_post(&palillos[philosopherId]);
    sem_post(&palillos[(philosopherId + 1) % NUM_PHILOSOPHERS]);
    sem_post(&lock);
    usleep(5000);
  }

  think(philosopherId);
  pthread_exit(NULL);
}


void Diner(pthread_t *threads)
{
    int i;
    for(i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&threads[i], NULL, philosopher, &id[i]);
    }

    for(i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}


int main(int argc, char *args[])
{
  InitVars(&lock, palillos);
  Diner(philosophers);
  pthread_exit(NULL);
}


void think(int position)
{
  printf("\nPhilosopher %d thinking...\n", position);
}

void eat(int position)
{
  printf(" \nPhilosopher %d eating... %s \n", position, hola);
}
