#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {

  printf("hola soy tu hijo\n" );
 printf("Ordenaremos: %d %d \n", atoi(argv[0]), atoi(argv[argc - 1]));

  int numeros[argc - 1];
  int n = argc - 1;

  int x;
  for ( x = 0; x < argc - 1; x++) {
      numeros[x] = atoi(argv[x]);
  }

  int i, j;
  for (i = 0; i < n-1; i++)
  {
   for (j = 0; j < n-i-1; j++)
   {
     if (numeros[j] > numeros[j+1])
     {
       int temp = numeros[j];
       numeros[j] = numeros[j + 1];
       numeros[j + 1] = temp;
     }
   }
  }

 for ( x = 0; x < argc - 1; x++)
 {
   printf("%d\n", numeros[x] );
 }

/* int shared_mem_key = 555555;
 int shm_id = shmget(shared_mem_key, SHM_SIZE, S_IRUSR | S_IWUSR);
 char * shmaddr = (char *) shmat(shm_id, NULL, 0);


 for ( x = 0; x < argc; x++)
 {
   strcpy( shmaddr[atoi(argv[argc - 1])],
 }
*/


  return 0;
}
