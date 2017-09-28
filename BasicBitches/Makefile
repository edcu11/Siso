all:
	g++ -shared -fPIC -o BasicFileSystem.so BasicFileSystem.cpp -lrt
	g++ -rdynamic -o get-file Getfile.cpp -ldl -lrt
	./get-file -f disk.sh alphonseJr.jpg


mainFile:
	gcc -rdynamic -o dynamicLibs dynamicLibs.c -ldl
	./dynamicLibs

dynamicLib:
	./dynamicLibs
