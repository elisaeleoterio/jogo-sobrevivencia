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
    float old_x;  
    int life; // de 0 a 5  
    int tipo;
} hitbox;

// Lista de obstáculos
typedef struct obstacle {
    struct hitbox *hitbox;
    struct obstacle *next;
} obstacle;

// Funções para tratar hitboxes normais
struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed_x, float speed_y, float forca_pulo, const char *filename, int tipo);
int verifica_colisao(struct hitbox *a, struct hitbox *b);
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key);
void desenha_hitbox(struct hitbox *a, float dest_x, float dest_y, 
                    float larg_final, float alt_final, int flag);
void destruir_hitbox(struct hitbox *a);

// Funções para tratar obstáculos em massa
void adicionar_obstaculo(struct obstacle **raiz, struct hitbox *hitbox);
void destruir_lista_obstaculos(struct obstacle *raiz);
void movimenta_lista_obstaculos(struct obstacle *raiz, ALLEGRO_KEYBOARD_STATE key);
void desenha_lista_obstaculos(struct obstacle *raiz);
void salva_pos_anterior_lista(struct obstacle *raiz);
void reverte_pos_lista(struct obstacle *raiz);

void verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key);
void verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos);

#endif
