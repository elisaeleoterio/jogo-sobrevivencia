#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

struct mundo {
    ALLEGRO_DISPLAY *display;
    int altura, largura; // Tamanho do monitor
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *fila_eventos
};

struct mundo *criar_mundo();
void inicializar();
ALLEGRO_DISPLAY *criar_display(int *largura, int *altura);