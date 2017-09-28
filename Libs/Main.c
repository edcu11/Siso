#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  typedef int (*operacion)(int, int);
  void* sharedObj = dlopen("/vagrant/Siso/Libs/lib.so", RTLD_LAZY);

  if(sharedObj == NULL)
  {
    printf("ERROR!\n");
    return 0;
  }

  operacion operacionDll = dlsym(sharedObj, "Operacion");

  printf("Operacion: %d\n", operacionDll(5,3));
  dlclose(sharedObj);

  return 0;
}
