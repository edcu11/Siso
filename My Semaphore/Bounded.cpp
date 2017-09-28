#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>


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
            while (done == 0)
            {
               // printf("Locked!");
                pthread_cond_wait(&Semaphore, &Lock);
               // printf("Finished Waiting!!");
            }
            done = 0;
        }
        pthread_mutex_unlock(&Lock);
        //printf("Released Mutex!");
    };

    void PostLegendary()
    {
        pthread_mutex_lock(&Lock);
        LockValue++;
        if(LockValue <= 0){
            done = 1;
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


MiSemaforo mutex, full, empty;
int buffer[100];
int arrayPos = 0;

void* producer(void * param)
{
  int randomNumber;
  while(1)
  {
    randomNumber = rand() % 1000;
    empty.WaitForIt();
    arrayPos = (empty.GetValue() > 100)? 99: empty.GetValue();
    mutex.WaitForIt();
    arrayPos++;
    printf("Produced: %d At: %d\n", randomNumber, arrayPos);
    buffer[arrayPos] = randomNumber;
    printf("PRUEBA%d\n", arrayPos);
    mutex.PostLegendary();
    full.PostLegendary();
    usleep(5000);
  }
}

void* consumer(void * id)
{

  int tid = *(int *)id;
  int numeroConsumido;
  while(1)
  {

    full.WaitForIt();
    arrayPos = (empty.GetValue() > 100)? 99:empty.GetValue();
    mutex.WaitForIt();
    arrayPos = arrayPos + 1;
    numeroConsumido = buffer[arrayPos];
    mutex.PostLegendary();
    empty.PostLegendary();
    printf("Consumed: %d, At: %d By: %d\n", numeroConsumido, arrayPos, tid);
    usleep(5000);
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


  for(counter = 0; counter < 100; counter++)
  {
    buffer[counter] = 0;
  }

  srand( (unsigned) time(NULL));

    mutex.Init(1);
    full.Init(0);
    empty.Init(cant);


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
