#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

void* cliente(void *id);
void* cajero(void *id);

sem_t mutx, full, empty;
int *buffer;
int i = 0;
int cant = 20;

int main(int argc, char *argv[]){
    int id[4] = {1, 2, 3, 4};
    buffer = (int*)malloc(sizeof(int) * (cant + 1));
    for(int c = 0; c < 10; c++){
        buffer[c] = 0;
    }
    srand( (unsigned) time(NULL));

    sem_init(&mutx, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, cant);

    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, cliente, NULL);

    pthread_attr_init(&attr);
    pthread_create(&tid2, &attr, cajero, &id[0]);

    pthread_attr_init(&attr);
    pthread_create(&tid3, &attr, cajero, &id[1]);

    pthread_attr_init(&attr);
    pthread_create(&tid4, &attr, cajero, &id[2]);

    pthread_attr_init(&attr);
    pthread_create(&tid5, &attr, cajero, &id[3]);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
}

void* cliente(void *id){
    //int id_con = *(int *)id;
    int num_pro;
    for(;;){
        num_pro = rand() % 500000;
        usleep(500000 + num_pro);
        sem_getvalue(&empty, &i);
        if(i == 0){
            printf("El cliente se fue por que el banco estaba lleno\n");
        }else{
            sem_wait(&empty);
            sem_wait(&mutx);
            i++;
            printf("Entro un cliente nuevo\n");
            buffer[i] = num_pro;
            sem_post(&mutx);
            sem_post(&full);
        }
    }
}

void* cajero(void * id){
    int id_con = *(int *)id;
    int num_con;
    int total;
    for(;;){
        sem_getvalue(&full, &total);
        if(total < cant * 0.5){
            printf("Cantidad de cajeros : 2\n");
            usleep(5000000);
        }else if(total < cant * 0.9){
            printf("Cantidad de cajeros : 3\n");
            usleep(5000000);
        }else{
            printf("Cantidad de cajeros : 4\n");
            usleep(5000000);
        }
        if((id_con == 1) || (id_con == 2) || (id_con == 3 && total >= cant * 0.5 && total < cant * 0.9) || (id_con == 4 && total >= cant * 0.9)){
            sem_wait(&full);
            sem_wait(&mutx);
            i = 20 - total;
            num_con = rand() % 1000000;
            usleep(1000000 + num_con);
            sem_post(&mutx);
            sem_post(&empty);
            printf("Cajero #%d atendio un cliente\n", id_con);
        }
    }
}
