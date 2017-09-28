all:
	gcc -rdynamic -o dynamicLibs Main.c -ldl
	gcc -shared -fPIC -o lib.so dllResta.c
	./dynamicLibs

mainFile:
	gcc -rdynamic -o dynamicLibs dynamicLibs.c -ldl
	./dynamicLibs

dynamicLib:
	gcc -shared -fPIC -o dll.so dll.c
	./dynamicLibs
