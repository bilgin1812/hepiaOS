SRC=main.c
OUT=qemu
CC=bcc

.PHONY: clean rebuild

$(OUT):	disk.img 
	@ echo "---> :qemu starts disk"
	qemu-system-i386 -hda disk.img

disk.img:./bootD/boot.img ./kernelD/kernel.img ./tools/sfs.img
	@echo "---> :Creating disk"
	dd if=/dev/zero of=disk.img bs=512 count=1000 
	dd conv=notrunc seek=0  if=./bootD/boot of=disk.img 
	dd conv=notrunc seek=1  if=./kernelD/kernel.img of=disk.img
	dd conv=notrunc seek=10 if=./tools/sfs.img of=disk.img
	 
	
./bootD/boot.img:
	make -C bootD

./kernelD/kernel.img:
	make -C kernelD

./tools/sfs.img:
	make -C tools  
	

clean:	
	rm -f *.o *.img
	make clean -C kernelD
	make clean -C bootD
	make clean -C tools

build:boot main.o kernel.img disk.img 

rebuild:clean build
	
