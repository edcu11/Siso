Ok, este es un pequeño tutorial para encaminarlos para completar la tarea 3.
Tomen en consideración que no me enfoco en eficiencia ni en limpieza (es chuco, pero funciona).
Recomiendo tener abierto todos los archivos incluidos con este read me, para que vayan leyendo el archivo al que hago referencia.

----------------------------------------------------------------------------------
Paso1: Boot -> Program Loader
Ok para empezar, hagamos que nuestro boot sector se pase a un diferente sector, que en el futuro será nuestro program loader, pero por ahora solo haremos que imprima una ‘S’ (if success) o una ‘F’ (if fail).
Para esto usaremos el archivo “boot.asm” y “loader.asm”

Primero, con el siguiente comando, creamos un floppy de 1.44 mb (tamaño estándar, puede ser más pequeño o más grande):
dd if=/dev/zero of=disk.flp bs=1024 count=1440

Luego compilamos nuestros archivos boot y loader:
nasm boot.asm -f bin -o boot.bin
nasm loader.asm -f bin -o loader.bin

Por último, los cargamos en el floppy que creamos anteriormente:
dd if=boot.bin of=disk.flp conv=notrunc
dd if=loader.bin of=disk.flp bs=512 seek=1 conv=notrunc

Nota que “conv=notrunc” es usado para que no agregue al final del archiva, si no adentro del floppy en sí. “seek=1” es utilizado para designar el sector al que escribiremos (‘1’ siendo el segundo sector).

Ahora podemos correr nuestro floppy:
qemu-system-x86_64 -fda disk.flp

Si aparece una ‘S’, todo salió bien, si no, algo salió mal. Talvez escribieron mal alguna instrucción, o estas no sirven en su distro de Linux. En todo caso, vuelvan a intentar, asegurando de escribir bien todo.
Perfecto, ahora que sabemos cómo movernos de un sector a otro, podemos pasar a las llamadas de C.
OPCIONAL: Les sugiero que copien y peguen su código de la primera tarea en el archivo ‘loader.asm’, menos el boot signatura al final, y hagan el procedimiento del paso 1. Si su tarea 1 corre de un solo, todo está bien, si no, puede ser que tengan que mover cosas del stack; háganlo hasta que funcione, para que aprendan como sirve este.

----------------------------------------------------------------------------------
Paso 2: Llamadas de C
Ok, para este ocuparemos otra vez nuestro archivo “boot.asm”, y además “program1.c” y “lib.asm” (este último lo subió el Ing. a BB).

Por motivos de no dejar basura en nuestro floppy, volvámoslo a armar:
dd if=/dev/zero of=disk.flp bs=1024 count=1440

Luego compilamos nuestro archivo boot:
nasm boot.asm -f bin -o boot.bin

Y ahora, compilaremos nuestro programa, junto con sus links a lib.asm:
as86 lib.asm -o kernel_asm.o
bcc -ansi -c program1.c -o kernel1.o
ld86 -0 -d kernel1.o kernel_asm.o
mv a.out kernel1

Bien, ahora ya tenemos nuestro ‘programa’ en kernel1, solo falta meterlo al floppy junto con el boot, como en el paso 1:
dd if=boot.bin of=disk.flp conv=notrunc
dd if=kernel1 of=disk.flp bs=512 conv=notrunc seek=1

Ahora podemos correr nuestro floppy:
qemu-system-x86_64 -fda disk.flp

Deberá de imprimir en pantalla “Fabian E. Canizales” 10 veces. Si tocan alguna tecla, lo volverá a imprimir 10 veces.
Perfecto, ya llamamos código de C, que a su vez llama código de assembly para imprimir cosas. Vamos bien. Lo único que falta es juntar el paso 1 y paso 2.

----------------------------------------------------------------------------------
Paso 3: boot -> program loader -> program -> program loader
El paso 2 simplemente llamaba al segundo sector, que contenía código de C, pero, nada más. Ahora haremos que ese código regrese al program loader, donde podremos cargar el código de C otra vez. Para esto, usaremos “boot.asm”, “loader2.asm”, “lib2.asm”, y “programa2.c”.

Otra vez, rehacemos el floppy:
dd if=/dev/zero of=disk.flp bs=1024 count=1440

Compilamos el boot y el program loader:
nasm boot.asm -f bin -o boot.bin
nasm loader2.asm -f bin -o loader.bin

Compilamos nuestro programa, junto con sus links a lib.asm:
as86 lib2.asm -o kernel_asm.o
bcc -ansi -c program2.c -o kernel2.o
ld86 -0 -d kernel2.o kernel_asm.o
mv a.out kernel2

Y metemos todo al floppy:
dd if=boot.bin of=disk.flp conv=notrunc
dd if=loader.bin of=disk.flp bs=512 seek=1 conv=notrunc
dd if=kernel2 of=disk.flp bs=512 conv=notrunc seek=2

Ahora podemos correr nuestro floppy:
qemu-system-x86_64 -fda disk.flp

Si todo salió bien, nuestro program loader nos preguntara que queremos cargar, considerando que solo tenemos una opción,
deberemos de escoger la primera, la cual cargara nuestro programa anterior, que imprime 10 veces “Fabian E. Canizales”. Después de eso, esperara un input para volver al program loader.

Bien, con esto ya prácticamente solo les faltaría inventarse 2 programas más, talvez ver cómo mover el stack un poco, si tienen problemas con sus otros programas.

Nota: “clear_screen” es una mula de chanchada, pone el puntero al principio, eh imprime 100 espacios, luego hace lo mismo en una nueva línea, 100 veces más, dando la impresión de un “clear screen”. Les dije, es chuco, pero funciona.
