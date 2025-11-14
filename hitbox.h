#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "erros.h"

#ifndef _HITBOX_H_
#define _HITBOX_H_

typedef struct hitbox {
    float y, x;
    float width, height;
    ALLEGRO_BITMAP *sprite; // Arquivo de sprite -> Tem que estar tratado (tamanho desejado e sem fundo)
    float speed_x, speed_y; // Velocidade de locomoção
    float forca_pulo;
    float chao;
    float steps;
} hitbox;

struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed_x, float speed_y, float forca_pulo, const char *filename);
int verifica_colisao(struct hitbox *a, struct hitbox *b);
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key);
void desenha_hitbox(struct hitbox *a, float dest_x, float dest_y, 
                    float larg_final, float alt_final, int flag);

#endif
