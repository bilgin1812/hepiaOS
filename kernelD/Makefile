SRC=boot.asm
OUT=build
CC=bcc
LINK=ld86
ASM=as86
CCFLAGS=-W -V -I -ansi -c
LINKFLAGS=-M -m -d -s -o

.PHONY: clean rebuild

$(OUT):main.o kernel.o  syscall.o fs.o  util_asm.o  	 	
	@ echo "---linking and creating kernel.img"
	$(LINK) $(LINKFLAGS) kernel.img main.o kernel.o fs.o syscall.o util_asm.o
	@echo "---> :Kernel Complied"
main.o:main.c
	@echo "---> :Compiling Kernel"
	$(CC) $(CCFLAGS) $< -o $@
kernel.o:kernel.c
	$(CC) $(CCFLAGS) $< -o $@

fs.o: fs.c
	$(CC) $(CCFLAGS)  $< -o $@

syscall.o: syscall.c
	$(CC) $(CCFLAGS)  $< -o $@

util_asm.o:
	@ echo "---> :compiling util_asm"
	$(ASM) util_asm.s -o util_asm.o


clean:
	rm -f kernel.o main.o syscall.o util_asm.o fs.o kernel.img 


rebuild:clean build



