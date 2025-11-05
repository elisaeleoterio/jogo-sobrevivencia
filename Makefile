CC = gcc
CFLAGS = -Wall -Wextra -Werror -g pkg-config allegro-5 allegro_font-5 --libs --cflags

all: survival

clean:
	rm -f *.o survival