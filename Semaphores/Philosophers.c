#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 10


int id[5];

sem_t lock;
sem_t palillos[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];

char * hola = "variable a leer, leida GG M8";

void think(int position);
void eat(int position);

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
    sem_wait(&palillos[philosopherId]);
    printf("%d Got first fork %d  .....................", philosopherId, philosopherId);
    sem_wait(&palillos[(philosopherId + 1) % NUM_PHILOSOPHERS]);
    printf("|||.................%d Got second fork %d ", philosopherId, philosopherId + 1);
    eat(philosopherId);
    sem_post(&palillos[philosopherId]);
    sem_post(&palillos[(philosopherId + 1) % NUM_PHILOSOPHERS]);
    sem_post(&lock);
  }

  think(philosopherId);
  pthread_exit(NULL);
}


void Diner(pthread_t *threads)
{
    int i;
    for(i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&threads[i], NULL, philosopher, &id[i]);
    }

    for(i = 0; i < NUM_PHILOSOPHERS; i++) {
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
  printf("\nPhilosopher %d thinking...........", position);
}

void eat(int position)
{
  printf(" \nPhilosopher %d eating... %s \n", position, hola);
}
