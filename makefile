# Nome do executável
EXEC = main

# Diretórios
SRC_DIR = src
OBJ_DIR = object

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Isrc -Iinclude
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Fontes e objetos
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Regra padrão
all: $(EXEC)

# Linkagem
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compilação dos .c em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rodar o programa
run: $(EXEC)
	./$(EXEC)

# Limpa os arquivos objetos e executável
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

# Limpa tudo, inclusive o diretório object
fclean:
	rm -rf $(OBJ_DIR) $(EXEC)

# Limpa e recompila tudo
re: fclean all
