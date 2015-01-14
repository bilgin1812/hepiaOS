SRC=boot.asm
OUT=boot
CC=nasm

.PHONY: clean rebuild

$(OUT):	 
	@echo "---> :Compiling boot"
	$(CC) $(SRC) -o boot
	

clean:
	rm -f boot 

build:boot 

rebuild:clean build