SRC=main.c
FLAGS=
OBJ=$(SRC:.asm=)
OUT=qemu
CC=bcc


$(OUT):	disk.img 
	qemu -hda disk.img

disk.img:boot kernel 
	dd if=/dev/zero of=disk.img bs=512 count=100 
	dd conv=notrunc seek=0 if=./bootD/boot of=disk.img 
	dd conv=notrunc seek=1 if=./kernelD/kernel of=disk.img 
boot: boot.asm
	nasm ./bootD/boot.asm 
kernel: kernel.asm
	nasm ./kernelD/kernel.asm 

%.o: %.c
	$(CC) $(FLAGS) -c $(SRC)   

clean:
	rm -f *.c *.o
	rm -f kernel boot disk.img
build  : boot kernel
rebuild: clean boot kernel
