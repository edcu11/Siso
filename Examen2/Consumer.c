#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>


sem_t mutex, full, empty;
int i = 0;
int casherCant = 10;

int sleepTime = 500000;

void* cliente(void *params){
    int numero;
    for(;;){
        numero = rand() % sleepTime;
        usleep(sleepTime + numero);
        sem_getvalue(&empty, &i);
        if(i == 0){
            printf("Bank is full, he left\n");
        }else{
            sem_wait(&empty);
            sem_wait(&mutex);
            printf("Client bought something\n");
            usleep(sleepTime);
            sem_post(&mutex);
            sem_post(&full);
        }
    }
}



void* cajero(void * id){
    int chaser = *(int *)id;
    int numero, total;
    for(;;){
        sem_getvalue(&full, &total);
        if(total < casherCant * 0.5)
        {
            printf("casherCantidad  : 1/2\n");
        }
        else if(total > casherCant * 0.5 && total < casherCant * 0.9)
        {
            printf("casherCantidad : 3/4\n");
        }
        else if ( total > casherCant *  0.9)
        {
            printf("casherCantidad  : N\n");
        }
        usleep(sleepTime);
        if((chaser == 1) || (chaser == 2) ||
          ( chaser == 3 && (total >= casherCant * 0.5 && total < casherCant * 0.9))
          || (chaser == 4 && total >= casherCant * 0.9))
          {
            sem_wait(&full);
            sem_wait(&mutex);
            numero = rand() % sleepTime;
            printf("Casher %d : sold something!\n", chaser);
            usleep(sleepTime + numero);
            sem_post(&mutex);
            sem_post(&empty);
        }
    }
}

void StartThread(pthread_t tid, pthread_attr_t attr, int *id)
{
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, cajero, id);
}

void StartThreadCliente(pthread_t tid, pthread_attr_t attr, int *id)
{
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, cliente, id);
}


int main(int argc, char *argv[]){

  int id[8];
  id[0] = 0;
  id[1] = 1;
  id[2] = 2;
  id[3] = 3;
  id[4] = 4;
  id[5] = 5;
  id[6] = 6;
  id[7] = 7;
  id[8] = 8;

  srand( (unsigned) time(NULL));

  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, casherCant);

  pthread_t tid[8];
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_create(&tid[0], &attr, cliente, NULL);

  int counter;
  for (counter = 0; counter < 4; counter++)
  {
    StartThreadCliente(tid[counter], attr, &id[counter]);
  }

  for (counter = 4; counter < 8; counter++)
  {
    StartThread(tid[counter], attr, &id[counter]);
  }


  for (counter = 0; counter < 8; counter++)
  {
    pthread_join(tid[counter], NULL);
  }

}
