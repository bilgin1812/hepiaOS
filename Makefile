SRC=main.c
FLAGS=-Wall -std=c99
OBJ=$(SRC:.c=.o)
OUT=test

$(OUT):	$(OBJ)
	gcc $(OBJ) $(OUT) 
%.o: %.c
	gcc $(FLAGS) -c $(SRC)   

clean:
	rm -f $(OBJ) $(OUT)

rebuild: clean $(OUT)
