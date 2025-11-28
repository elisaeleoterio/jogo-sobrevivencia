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
    L_FIM,
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
    T_FIM,
};

// Enumeração dos Estados do Player (Sprites) ---
enum {
    OBSTACULOS, // Para armazenar sprites normais dos obstáculos
    S_IDLE,   // Parado
    S_WALK,   // Andando
    S_JUMP,   // Pulando    
    S_FLY,    // Voando
    S_CROUCH, // Abaixado
    NUM_STATES // Total de estados
};

typedef struct hitbox {
    float y, x;
    float width, height;
    
    // Vetor de imagens (Sprite Sheets)
    ALLEGRO_BITMAP *sprites[NUM_STATES]; 
    int max_frames[NUM_STATES];
    int current_frame;
    int frame_timer;
    int frame_delay;
    int current_state;
    int direction;

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

void configura_sprites_player(struct hitbox *p, ALLEGRO_BITMAP *idle, int f_idle,  ALLEGRO_BITMAP *walk, int f_walk, ALLEGRO_BITMAP *jump, 
                                int f_jump, ALLEGRO_BITMAP *fly, int f_fly, ALLEGRO_BITMAP *crouch, int f_crouch);
void desenha_personagem(struct hitbox *p, int flag);
void atualiza_animal(struct hitbox *obs);
void atualiza_nuvem_movel(struct hitbox *obs);

#endif
