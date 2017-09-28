#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

void* reader();
void* writer();

sem_t mutex, semRW;
int read_count = 0;
int numeros;

void StartThread(pthread_t tid, pthread_attr_t attr, int *id)
{
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, reader, id);
}

void StartIdArray(int array[])
{
      int counter;
      for (counter = 1; counter < 5; counter++)
      {
        array[counter] = counter;
        printf("%d\n", array[counter]);
      }

}

void* writer(){
  int x = 0;
  while(1){
    x++;
    sem_wait(&semRW);
    printf("wrote: %d\n", x);
    numeros = x;
    sem_post(&semRW);
    usleep(5000);
  }
}

void* reader(void *tid){
  int rid = *(int *)tid;
  while(1){
    sem_wait(&mutex);
    read_count++;
    if(read_count == 1)
      sem_wait(&semRW);
    sem_post(&mutex);
    printf("Reader %d got, %d\n", rid, numeros);
    sem_wait(&mutex);
    read_count--;
    if(read_count == 0)
      sem_post(&semRW);
    sem_post(&mutex);
    usleep(5000);
  }
}

int main(){
    sem_init(&mutex, 0, 1);
    sem_init(&semRW, 0, 1);
    int id[5];

    StartIdArray(id);

    pthread_t tid[5];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, writer, NULL);

    int counter;
    for (counter = 1; counter < 5; counter++)
    {
        printf("uyoo %d", counter );
        StartThread(tid[counter], attr, &id[counter]);
    }

    for (counter = 0; counter < 5; counter++)
    {
      pthread_join(tid[counter], NULL);
    }
}
