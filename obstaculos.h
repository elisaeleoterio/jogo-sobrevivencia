#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h" // Incluir hitbox para saber o tamanho da struct

#ifndef _OBSTACULOS_H_
#define _OBSTACULOS_H_

typedef struct obstacle {
    struct hitbox *hitbox;
    struct obstacle *next;
} obstacle;

void adicionar_obstaculo(struct obstacle **raiz, struct hitbox *hitbox);
void destruir_lista_obstaculos(struct obstacle *raiz);
void movimenta_lista_obstaculos(struct obstacle *raiz, ALLEGRO_KEYBOARD_STATE key);
void desenha_lista_obstaculos(struct obstacle *raiz);
void salva_pos_anterior_lista(struct obstacle *raiz);
void reverte_pos_lista(struct obstacle *raiz);

bool verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key);
bool verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos);
bool colide_obs(struct hitbox *a, struct obstacle *b);

void atualiza_lista_buracos(struct obstacle *raiz);
void atualiza_lista_animais(struct obstacle *raiz);
void atualiza_lista_nuvem_movel(struct obstacle *raiz);

#endif