SRC=main.c
OUT=qemu
CC=bcc

.PHONY: clean rebuild

$(OUT):	disk.img 
	@ echo "---> :qemu starts disk"
	qemu-system-i386 -hda disk.img

disk.img:boot  kernel.img
	@echo "---> :Creating disk"
	dd if=/dev/zero of=disk.img bs=1024 count=100 
	dd conv=notrunc seek=0 if=boot of=disk.img 
	dd conv=notrunc seek=1 if=kernel.img of=disk.img 

boot:./bootD/boot.asm
	@echo "---> :Compiling boot"
	nasm ./bootD/boot.asm -o boot

kernel.img:main.o kernel.o util_asm.o
	@echo "---> :Compiling main and kernel " 	
main.o:
	$(CC) -W -V -I -ansi -c ./kernelD/main.c -o main.o
kernel.o:
	$(CC) -W -V -I -ansi -c ./kernelD/kernel.c -o kernel.o
util_asm.o:
	@ echo "---> :compiling util_asm"
	as86 ./kernelD/util_asm.s -o util_asm.o
	@ echo "---linking and creating kernel.img"
	ld86 -M -m -d -s -o kernel.img main.o kernel.o util_asm.o
 

clean:
	rm -f kernel.o main.o util_asm.o  kernel.img boot disk.img
	rm -f *.o *.img

build:boot main.o kernel.o util_asm.o kernel.img disk.img 

rebuild:clean build
	
