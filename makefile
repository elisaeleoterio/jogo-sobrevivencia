CC = gcc
# Define uma variável para os pacotes, facilitando a leitura
PKGS = allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5
# Flags de COMPILAÇÂO
CFLAGS = $(shell pkg-config --cflags $(PKGS)) -Wall -Wextra
#Flags de LINKAGEM
LDLIBS = $(shell pkg-config --libs $(PKGS))


# Lista de arquivos fonte
SRC = main.c telas.c erros.c inicio.c

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

main.o: telas.h erros.h inicio.h
telas.o: telas.h
erros.o: erros.h	
inicio.o: inicio.h

clean:
	rm -f *.o $(TARGET)