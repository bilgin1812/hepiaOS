CC=gcc
FLAGS=
OUT=sfs.img
SRC=main.c 
OBJ=$(SRC:.c=.o)

$(OUT): $(OBJ)
	@ echo "---linking and creating sfs.img"
	$(CC) $(SRC) 
	./a.out

.PHONY: clean rebuild

clean:
	rm-f $(OBJ) $(OUT)

rebuild: clean $(OBJ)
