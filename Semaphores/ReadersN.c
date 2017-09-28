#include <stdio.h>
#include <semaphore.h>


sem_t mutex, semRw;
int read_count = 0;
int numeros = 0;

void * Writer()
{
    int x = 0;
    do {
        x++;
        sem_wait(&semRw);
        numeros = x;
        printf("Wrote: %d \n", x);
        sem_post(&semRw);
     } while (x < 200);

}

void * Reader(void *tid)
{
    int rid = *(int *)tid;
    int x = 0;
    do {
        sem_wait(&mutex);
        read_count++;
        printf("%d",read_count);
        if (read_count == 1)
            sem_wait(&semRw);
        sem_post(&mutex);

        printf("%d Got: %d \n", rid, numeros);

        sem_wait(&mutex);
        read_count -= 1;
        printf("%d",read_count);
        if (read_count == 0)
            sem_post(&semRw);
        sem_post(&mutex);
        x++;
    } while (x< 200);

}


void StartThread(pthread_t tid, pthread_attr_t attr, int *id)
{
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, Reader, id);
}

int main()
{
    pthread_t tid[5];
    pthread_attr_t attr;
    int id[5];
    id[1] = 1;
    id[2] = 2;
    id[3] = 3;
    id[4] = 4;

    sem_init(&mutex, 0, 1);
    sem_init(&semRw, 0, 1);

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, Writer, NULL);


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

    return 0;
}
