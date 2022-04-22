CC = gcc
SRC = src/
OBJ = objects/
BIN = bin/

$(BIN)create: $(SRC)*
	$(CC) -o $@ $^ 

clean:
	rm -rf ../Test