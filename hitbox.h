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


    // NOVOS CAMPOS PARA OBTÁCULO COM MOVIMENTO AUTOMÁTICO
    float patrol_speed;  // Velocidade própria do objeto (independente do player)
    float min_x, max_x;  // Limites da área de patrulha
    float old_min_x, old_max_x; 
    int patrol_dir;      // 1 (direita/cima) ou -1 (esquerda/baixo)
    
    // CAMPOS PARA BURACOS QUE SE MOVEM VERTICALMENTE
    float min_y, max_y;
    float old_min_y, old_max_y;
    
    // CAMPOS PARA BURACO QUE SURGE
    int timer;
    bool active;
    int interval;

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

bool verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key);
bool verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos);

bool colide_obs(struct hitbox *a, struct obstacle *b);

void configura_patrulha(struct hitbox *obs, float distancia_patrulha, float velocidade_propria);
void atualiza_patrulha(struct hitbox *obs);
void atualiza_lista_patrulha(struct obstacle *raiz);

void configura_buraco_periodico(struct hitbox *obs, int intervalo_frames);
void atualiza_buracos_periodicos(struct obstacle *raiz);

void atualiza_lista_plat_movel(struct obstacle *raiz);
void atualiza_patrulha(struct hitbox *obs);
void configura_plat_movel(struct hitbox *obs, float distancia_altura, float velocidade_propria);

void atualiza_lista_obs_despenca(struct obstacle *raiz);
void obs_despenca_atualiza(struct hitbox *obs);
void obs_despenca_horizontal(struct hitbox *obs, int direcao, float velocidade_propria);

#endif
