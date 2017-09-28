#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 10

class MiSemaforo
{
    pthread_cond_t Semaphore;
    pthread_mutex_t Lock;
    int done;
    int LockValue;

    public:
    void Init(int ValorInicial)
    {
        pthread_cond_t Semaphore = PTHREAD_COND_INITIALIZER;
        pthread_mutex_t Lock = PTHREAD_MUTEX_INITIALIZER;
        LockValue = ValorInicial;
        done = 0;
        printf("Semaphore Initialized Correctly!\n");
    }

    void WaitForIt()
    {
        pthread_mutex_lock(&Lock);
        LockValue--;
        if(LockValue < 0)
        {
            while (done < 1)
            {
               // printf("Locked!");
                pthread_cond_wait(&Semaphore, &Lock);
               // printf("Finished Waiting!!");
            }
            done--;
        }
        pthread_mutex_unlock(&Lock);
        //printf("Released Mutex!");
    };

    void PostLegendary()
    {
        pthread_mutex_lock(&Lock);
        LockValue++;
        if(LockValue <= 0){
            done++;
            pthread_cond_signal(&Semaphore);
        }
       // printf("Posted!");
        pthread_mutex_unlock(&Lock);
    };

    int GetValue()
    {
        return LockValue;
    }

};





int id[5];

MiSemaforo lock;
MiSemaforo palillos[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];

//char * hola = "variable a leer, leida GG M8";

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

    palillos[(philosopherId + 1) % NUM_PHILOSOPHERS].WaitForIt();
    printf("%d Am Even! Got first fork %d  ---------------", philosopherId, (philosopherId + 1) % NUM_PHILOSOPHERS);
    palillos[philosopherId].WaitForIt();
    printf("|||-----------%d Got second fork %d ", philosopherId, philosopherId);

    return;
}

void pickOddForks(int philosopherId)
{

    palillos[philosopherId].WaitForIt();
    printf("%d Am Odd! Got first fork %d  ---------------", philosopherId, philosopherId);
    palillos[(philosopherId + 1) % NUM_PHILOSOPHERS].WaitForIt();
    printf("|||-----------%d Got second fork %d ", philosopherId, (philosopherId + 1) % NUM_PHILOSOPHERS);
    return;
}


void InitVars(MiSemaforo lock, MiSemaforo *palillos)
{
  int i;
  for(i = 0; i < NUM_PHILOSOPHERS; i++) {
    palillos[i].Init(1);
    id[i] = i;
  }

  lock.Init(NUM_PHILOSOPHERS - 1);
}


void *philosopher(void *params)
{
  int i;
  int philosopherId = *(int *)params;
  printf("Philosopher #%d started!", philosopherId);

  for(i = 0; i < 10; ) {
    think(philosopherId);

    //printf("locked!:");
    if(IsEven(philosopherId))
    {
        pickEvenForks(philosopherId);
    }
    else
    {
        pickOddForks(philosopherId);
    }
    //lock.WaitForIt();
    eat(philosopherId);
    palillos[philosopherId].PostLegendary();
    palillos[(philosopherId + 1) % NUM_PHILOSOPHERS].PostLegendary();
    //usleep(5000);
   // lock.PostLegendary();
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
  InitVars(lock, palillos);
  Diner(philosophers);
  pthread_exit(NULL);
}


void think(int position)
{
  printf("\nPhilosopher %d thinking...\n", position);
}

void eat(int position)
{
  printf(" \nPhilosopher %d eating...  \n", position);
}
