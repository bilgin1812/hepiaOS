CC=gcc
FLAGS=
OUT=build
SRC=main.c 
OBJ=$(SRC:.c=.o)

$(OUT): $(OBJ)
	@ echo "---linking and creating sfs.img"
	$(CC) main.c -o main
	./main

.PHONY: clean rebuild

clean:
	rm -f main.o

rebuild: clean $(OUT)
