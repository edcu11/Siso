all:
	dd if=/dev/zero of=disk.flp bs=512 count=500
	nasm boot.asm -f bin -o boot.bin
	nasm menu.asm -f bin -o loader.bin

	as86 lib2.asm -o kernel_asm.o

	bcc -ansi -c format.c -o format.o
	ld86 -0 -d format.o kernel_asm.o -o format.bin

	bcc -ansi -c Stats.c -o stats.o
	ld86 -0 -d stats.o kernel_asm.o -o stats.bin

	bcc -ansi -c Alocar.c -o get_block.o
	ld86 -0 -d get_block.o kernel_asm.o -o get_block.bin

	bcc -ansi -c Liberar.c -o free_block.o
	ld86 -0 -d free_block.o kernel_asm.o -o free_block.bin

	dd if=boot.bin of=disk.flp conv=notrunc
	dd if=loader.bin of=disk.flp bs=512 seek=1 conv=notrunc
	dd if=format.bin of=disk.flp bs=512 conv=notrunc seek=2
	dd if=stats.bin of=disk.flp bs=512 conv=notrunc seek=4
	dd if=get_block.bin of=disk.flp bs=512 conv=notrunc seek=6
	dd if=free_block.bin of=disk.flp bs=512 conv=notrunc seek=10

new:
