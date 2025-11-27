#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h"
#include "inicio.h"

struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed_x, ALLEGRO_BITMAP *sprite, int tipo) {
    struct hitbox *hit = calloc(1, sizeof(struct hitbox));
    if (!hit) {
        matar_falta_memoria();
    }

    // Definições específicas
    hit->x = x;
    hit->y = y;
    hit->width = w;
    hit->height = h;
    hit->sprite = sprite;
    hit->speed_x = speed_x;
    hit->tipo = tipo;

    // Definições gerais
    hit->old_x = x;
    
    return hit;
}

// CONFIGURAÇÕES EXTRAS DAS HITBOXES

void configura_player(struct hitbox *hit, float speed_y, float forca_pulo) {
    if (!hit) {
        matar_pont_nulo();
    }

    // Características específicas
    hit->speed_y = speed_y;
    hit->forca_pulo = forca_pulo;
        
    // Características gerais
    hit->chao = true;
    hit->steps = 0;
    hit->life = 5;
    hit->max_fly = 180; // 180 frames = 3 segundos de voo
    hit->fly_timer = hit->max_fly; // Começa cheio
    hit->def_height = hit->height;
}

void configura_animal(struct hitbox *obs, float distancia, float velocidade_propria) {
    if (!obs) {
        matar_pont_nulo();
    }

    obs->min_x = obs->x;
    obs->max_x = obs->x + distancia;
    obs->mov_speed = velocidade_propria;
    obs->mov_speed = 1; // Começa indo para a direita
}

void configura_buraco(struct hitbox *obs, int intervalo, bool active) {
    if (!obs) {
        matar_pont_nulo();
    }

    obs->active = active;
    obs->timer = 0;
    obs->interval = intervalo;
}

void configura_nuvem_movel(struct hitbox *obs, float distancia_altura, float velocidade) {
    if (!obs) {
        matar_pont_nulo();
    }

    obs->min_y = obs->y;
    obs->max_y = obs->y + distancia_altura;
    obs->mov_speed = velocidade;
    obs->mov_direcao = 1;
}

// FUNÇÕES DE MANIPULAÇÃO DAS HITBOXES

// Retorna 1 se houver colisão e 0 se não houver
int verifica_colisao(struct hitbox *a, struct hitbox *b) {
    if (!a || !b) {
        matar_pont_nulo();
    }

    // a está totalmente a esquerda de b
    if (a->x + a->width < b->x) {
        return 0;
    }
    // a está totalmente a direita de b
    if (a->x > b->x + b->width) {
        return 0;
    }
    // a está totalmente acima de b
    if (a->y + a->height < b->y) {
        return 0;
    }
    // a está totalmente abaixo de b
    if (a->y > b->y + b->height) {
        return 0;
    }
    return 1;    
}

// Altera as propriedade da hitbox para que se movimente
// TODO: VER SPRITES
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        a->x += a->speed_x;

        if (a->tipo == T_ANIMAL) {
            a->min_x += a->speed_x;
            a->max_x += a->speed_x;
        }        
        a->steps++;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        a->x -= a->speed_x;

        if (a->tipo == T_ANIMAL) {
            a->min_x -= a->speed_x;
            a->max_x -= a->speed_x;
        }
        a->steps--;  
    }
    // bool agachado = false;
    if (al_key_down(&key, ALLEGRO_KEY_DOWN) && a->chao) {
        // Só agacha se estiver no chão
        // Se ainda está em pé, agacha
        if (a->height == a->def_height) {
            float metade_altura = a->def_height / 2.0;
            // Ajusta o Y para que os pés continuem no chão ao encolher
            a->y += (a->def_height - metade_altura); 
            a->height = metade_altura;
        }
        // agachado = true;
    } else {
        // LEVANTAR
        // Se soltou a tecla e está agachado (altura menor que o padrão)
        if (a->height < a->def_height) {
            // Ajusta o Y para crescer
            a->y -= (a->def_height - a->height);
            a->height = a->def_height;
            // agachado = false;
        }
    }
    if (al_key_down(&key, ALLEGRO_KEY_UP) && a->chao) {
        a->speed_y += a->forca_pulo;
        a->chao = false; 
    }
    if (al_key_down(&key, ALLEGRO_KEY_SPACE) && a->fly_timer > 0) {
        a->speed_y = -5;
        a->fly_timer--;
        a->chao = false;
    }
    // Recarregar combustível se estiver no chão
    if (a->chao && a->fly_timer < a->max_fly) {
        a->fly_timer += 0.1;
        if (a->fly_timer > a->max_fly) {
            a->fly_timer = a->max_fly;
        }
    }
}

void desenha_hitbox(struct hitbox *a) {
    if (!a) {
        matar_pont_nulo();
    }

    if (!a->sprite) {
        return;
    }

    // offset_topo: "sobe" visualmente.
    float offset_topo = 35.0; 
    float offset_base = 10.0;  // Estende a imagem um pouco para baixo

    // ajuste_lateral: Controla a largura visual
    //   > 0 (Positivo): Alarga a imagem (borda extra)
    //   < 0 (Negativo): Estreita a imagem (encolhe visualmente)
    //   = 0 (Zero): Largura exata da hitbox
    float ajuste_lateral = -30.0; // Deixa 5 pixels mais estreito de cada lado

    // O Sprite deve ser desenhado da origem dele (0,0) até sua largura/altura total
    // E ser colocado na posição de destino (dest_x, dest_y)
    al_draw_scaled_bitmap(a->sprite, 0, 0, al_get_bitmap_width(a->sprite), al_get_bitmap_height(a->sprite), a->x - ajuste_lateral, a->y - offset_topo, a->width + ajuste_lateral * 2, a->height + offset_topo + offset_base, 0);
}

void destruir_hitbox(struct hitbox *a) {
    if (!a) {
        matar_pont_nulo();
    }
    free(a);
}

// Faz o objeto andar sozinho dentro dos limites
void atualiza_animal(struct hitbox *obs) {
    if (!obs) {
        return;
    }

    // Move baseado na velocidade própria e direção atual
    obs->x += obs->mov_speed * obs->mov_direcao;

    // Verifica limites e inverte a direção
    if (obs->x <= obs->min_x) {
        obs->x = obs->min_x;
        obs->mov_direcao = 1; // Vira para direita
    }
    else if (obs->x >= obs->max_x) {
        obs->x = obs->max_x;
        obs->mov_direcao = -1; // Vira para esquerda
    }
}

// Faz o objeto se mover sozinho dentro dos limites
void atualiza_nuvem_movel(struct hitbox *obs) {
    if (!obs) {
        return;
    }

    // Move baseado na velocidade própria e direção atual
    obs->y += obs->mov_speed * obs->mov_direcao;

    // Verifica limites e inverte a direção
    if (obs->y <= obs->min_y) {
        obs->y = obs->min_y;
        obs->mov_direcao = 1; // Vira para cima
    }
    else if (obs->y >= obs->max_y) {
        obs->y = obs->max_y;
        obs->mov_direcao = -1; // Vira para baixo
    }
}