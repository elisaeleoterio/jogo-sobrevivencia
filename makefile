CC = gcc
# Define uma variável para os pacotes, facilitando a leitura
PKGS = allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_primitives-5
# Flags de COMPILAÇÂO
CFLAGS = $(shell pkg-config --cflags $(PKGS)) -Wall -Wextra
#Flags de LINKAGEM
LDLIBS = $(shell pkg-config --libs $(PKGS)) -lm


# Lista de arquivos fonte
SRC = main.c fase_zero.c erros.c inicio.c hitbox.c menu.c obstaculos.c

# Lista de arquivos objetos
OBJS = $(SRC:.c=.o)

# Nome do executável
TARGET = survival

all: $(TARGET)

# Regra de linkagem.
# $^ (significa "todas as dependências", ou seja, todos os .o)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $^ $(LDLIBS)

# Regra de padrão para compilar C
# $< (significa "a primeira dependência", ou seja, o .c)
# $@ (significa "o alvo", ou seja, o .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c fase_zero.h erros.h inicio.h
fase_zero.o: fase_zero.h fase_zero.c
erros.o: erros.h erros.c
inicio.o: inicio.h inicio.c
hitbox.o: hitbox.h hitbox.c
menu.o: menu.h menu.c
obstaculos.o = obstaculos.h obstaculos.c

clean:
	rm -f *.o $(TARGET)