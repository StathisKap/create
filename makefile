CC = gcc
SRC = src/
OBJ = objects/
BIN = bin/
INCLUDE = -Iinclude/ 
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

$(BIN)create: $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ 

$(OBJECTS): $(OBJ)%.o : $(SRC)%.c
	@mkdir -p $(@D)
    $(CC) -c $< -o $@
#$(OBJ)%.o: $(SRC)%.c $(INCLUDE)
#	@mkdir -p $(@D)
#	$(CC) -c -o $@ $< 

clean:
	rm -rf ../Test