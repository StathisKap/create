CC = gcc
SRC = src/
OBJ = objects/
BIN = bin/
SOURCES = $(wildcard $(SRC)*.c)
OBJECTS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES))

$(BIN)create: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ 

$(OBJECTS): $(OBJ)%.o : $(SRC)%.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ 

clean:
	rm -rf ../Test
	rm -rf $(OBJ)
	rm $(BIN)create