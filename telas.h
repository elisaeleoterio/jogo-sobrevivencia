#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>

#include "erros.h"
#include "inicio.h"

int menu(struct mundo *mundo);
int fase_zero(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, int altura, int largura);
int fase_um(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, int altura, int largura);
int fase_dois(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_TIMER *timer, int altura, int largura);
