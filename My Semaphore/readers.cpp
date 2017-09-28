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


void* reader(void *param);
void* writer(void *param);

MiSemaforo lock, semRw;
int readers = 0;
int num;

void InitArray(int array[], int cant)
{
    int x;
    for(x = 0 ; x < cant; x++)
    {
        array[x] = x;
    }

}


int main(){
    lock.Init(1);
    semRw.Init(1);
    int id[5];
    InitArray(id, 5);

    pthread_t tid[5];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, writer, NULL);

    int x;
    for(x = 1; x < 5; x++)
    {
        pthread_attr_init(&attr);
        pthread_create(&tid[x], &attr, reader, &id[x]);
    }

    for(x = 0; x < 5; x++)
    {
        pthread_join(tid[x], NULL);
    }
}

void* writer(void *param){
    while(1){
        semRw.WaitForIt();
        num++;
        printf("Wrote: %d\n", num);
        semRw.PostLegendary();
        usleep(5000);
    }
}

void* reader(void *param){
    int id = *(int *)param;
    while(1){
        lock.WaitForIt();
        readers++;
        if(readers == 1)
            semRw.WaitForIt();
        lock.PostLegendary();
        printf("Reader %d: %d\n", id, num);
        lock.WaitForIt();
        readers--;
        if(readers == 0)
            semRw.PostLegendary();
        lock.PostLegendary();
                usleep(5000);
    }
}
