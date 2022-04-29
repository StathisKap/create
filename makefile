CC = gcc
SRC = src/
OBJ = objects/
BIN = bin/
SOURCES = $(wildcard $(SRC)*.c)
OBJECTS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(SOURCES))
INSTALL_DIR = ${HOME}/.local/create/
UNAME_S := $(shell uname -s)

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

install: $(OBJECTS)
	$(shell echo "export PATH=\"$(INSTALL_DIR):\$$PATH\"" >> ${HOME}/.zshrc)
	@mkdir -p $(INSTALL_DIR)
	$(CC) -o $(INSTALL_DIR)/create $^
	cp -f ./bin/github_new_repo.py $(INSTALL_DIR)
	cp -f ./bin/secrets.py $(INSTALL_DIR)
	@echo "Installed in:"
	@echo $(INSTALL_DIR)
