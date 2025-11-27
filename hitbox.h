#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "erros.h"

#ifndef _HITBOX_H_
#define _HITBOX_H_

// Enumeração dos tipos de obstáculos
enum { 
    L_NUVEM,
    L_ESPINHO,
    L_ANIMAL,
    L_BURACO,
    L_NUVEM_MOVEL,
    L_PRENSA,
    L_TEMPESTADE,
    NUM_LISTAS
};

// Enumeração dos tipos de hitboxes
enum {
    T_PERSONAGEM,
    T_NUVEM,
    T_ESPINHO,
    T_ANIMAL,
    T_BURACO,
    T_NUVEM_MOVEL,
    T_TEMPESTADE,
    T_ROLAMENTOS,
};

typedef struct hitbox {
    float y, x;
    float width, height;
    ALLEGRO_BITMAP *sprite; // Arquivo de sprite
    float speed_x, speed_y; // Velocidade de locomoção
    float forca_pulo;
    float chao;
    float steps;
    float old_x;  
    int life; // de 0 a 5  
    int tipo;

    // Ação de interação (voo)
    float fly_timer;
    float max_fly;

    // Ação de abaixar
    float def_height;


    // NOVOS CAMPOS PARA OBTÁCULO COM MOVIMENTO AUTOMÁTICO
    float mov_speed;  // Velocidade própria do objeto (independente do player)
    float min_x, max_x;  // Limites da área de patrulha
    float old_min_x, old_max_x; 
    int mov_direcao;      // 1 (direita/cima) ou -1 (esquerda/baixo)
    
    // CAMPOS PARA BURACOS QUE SE MOVEM VERTICALMENTE
    float min_y, max_y;
    float old_min_y, old_max_y;
    
    // CAMPOS PARA BURACO QUE SURGE
    int timer;
    bool active;
    int interval;

} hitbox;

// Funções para tratar hitboxes normais
struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed_x, ALLEGRO_BITMAP *sprite, int tipo);

void configura_player(struct hitbox *hit, float speed_y, float forca_pulo);
void configura_animal(struct hitbox *obs, float distancia, float velocidade_propria);
void configura_buraco(struct hitbox *obs, int intervalo, bool active);
void configura_nuvem_movel(struct hitbox *obs, float distancia_altura, float velocidade);

int verifica_colisao(struct hitbox *a, struct hitbox *b);
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key);
void desenha_hitbox(struct hitbox *a);
void destruir_hitbox(struct hitbox *a);


void atualiza_animal(struct hitbox *obs);
void atualiza_nuvem_movel(struct hitbox *obs);

#endif
