//gcc 5.4.0

#include  <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NTHREADS 8

int array[64];
int tamanoParticion;

void * sort(void *param);
void PutValues(char * argv[], int argc)
{
	int cont;
	for (cont = 0; cont < argc - 1; cont++)
	{
		array[cont] = atoi(argv[cont + 1]);
	}

}

void PrintValues(int argc)
{
	int cont;
	for (cont = 0; cont < argc - 1; cont++)
	{
		printf("--%d-- ", array[cont]);
		if ((cont + 1) % 4 == 0)
			printf("\n");
	}
}

void ActualSort(int init, int last)
{

	int i, j, min_idx;

	// One by one move boundary of unsorted subarray
	for (i = init; i < last - 1; i++)
	{
		// Find the minimum element in unsorted array
		min_idx = i;
		for (j = i + 1; j < last; j++)
		{
			if (array[j] < array[min_idx])
			{
				min_idx = j;
			}
		}

		// Swap the found minimum element with the first element
		int temp = array[i];
		array[i] = array[min_idx];
		array[min_idx] = temp;
	}

}

void * Sort(void *args)
{
	long init;
	init = (long)args;
	printf("thread: #%ld! pos: %d\n", init, array[init]);

	ActualSort(init, init + tamanoParticion);
	pthread_exit(NULL);
}

void * Union(void *args)
{
	long inicioParticion;
	inicioParticion = (long)args * tamanoParticion;
	printf("Onion thread #%ld!\n", inicioParticion);

	ActualSort(inicioParticion, inicioParticion + (tamanoParticion * 2));

	pthread_exit(NULL);
}



void CreateSortThreads(pthread_t threads[])
{
	long t;
	int rc;
	for (t = 0; t <= NTHREADS; t++)
	{
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, Sort, (void *)(t * tamanoParticion));
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (t = 0; t < NTHREADS; t++)
	{
		rc = pthread_join(threads[t], NULL);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

}

void CreateUnionThreads(pthread_t threads[], long Limite)
{
	long t;
	long ts = 0;
	int rc;
	for (t = 0; t <= Limite; t++)
	{
		ts = t * 2;
		printf("In main: creating thread %ld\n", ts);
		rc = pthread_create(&threads[t], NULL, Union, (void *)(ts));
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	ts = 0;
	for (t = 0; t <= Limite; t++)
	{
		rc = pthread_join(threads[t], NULL);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}

}



int main(int argc, char *argv[])
{
	pthread_t threads[NTHREADS];
	PutValues(argv, argc);

	if ((argc - 1) % 8 != 0)
	{
		printf("Ingrese una cantidad de numeros divisibles en 8!");
		return 0;
	}

	tamanoParticion = (argc - 1) / 8;
	CreateSortThreads(threads);
	PrintValues(argc);

	int threadCont;
	for (threadCont = 6; threadCont > 0; threadCont -= 2)
	{
		CreateUnionThreads(threads, threadCont);
		tamanoParticion += tamanoParticion;
		PrintValues(argc);
	}

	pthread_exit(NULL);
}
