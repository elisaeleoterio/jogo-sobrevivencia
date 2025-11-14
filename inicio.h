#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#ifndef MUNDO_H_
#define MUNDO_H_
struct mundo {
    ALLEGRO_DISPLAY *display;
    int altura, largura; // Tamanho do monitor
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    float gravidade;
};

struct mundo *criar_mundo();
void destruir_mundo(struct mundo *mundo);
void inicializar();
ALLEGRO_DISPLAY *criar_display(int *largura, int *altura);
void movimenta_background(float *back_x, float velocidade, ALLEGRO_KEYBOARD_STATE key);

#endif