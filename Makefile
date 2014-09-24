SRC=main.c
FLAGS=
OBJ=$(SRC:.asm=)
OUT=qemu
CC=bcc

$(OUT):	disk.img 
	qemu -hda disk.img

disk.img:boot kernel 
	dd if=/dev/zero of=disk.img bs=512 count=100 
	dd conv=notrunc seek=0 if=boot of=disk.img 
	dd conv=notrunc seek=1 if=kernel of=disk.img 
boot: boot.asm
	nasm boot.asm
kernel: kernel.asm
	nasm kernel.asm

%.o: %.c
	$(CC) $(FLAGS) -c $(SRC)   

clean:
	rm -f *.c *.o
	rm -f kernel boot disk.img

rebuild: clean $(OUT)
