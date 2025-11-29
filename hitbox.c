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
    hit->sprites[OBSTACULOS] = sprite; // Por padrão, é obstáculo
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
    hit->frame_delay = 15;
}

void configura_sprites_player(struct hitbox *p, ALLEGRO_BITMAP *idle, int f_idle, ALLEGRO_BITMAP *walk, int f_walk, ALLEGRO_BITMAP *jump,
                             int f_jump, ALLEGRO_BITMAP *fly, int f_fly, ALLEGRO_BITMAP *crouch, int f_crouch) {
    if (!p) {
        matar_pont_nulo();
    }
    
    p->sprites[S_IDLE] = idle;
    p->max_frames[S_IDLE] = f_idle;
    p->sprites[S_WALK] = walk;
    p->max_frames[S_WALK] = f_walk;
    p->sprites[S_JUMP] = jump;
    p->max_frames[S_JUMP] = f_jump;
    p->sprites[S_FLY] = fly;
    p->max_frames[S_FLY] = f_fly;
    p->sprites[S_CROUCH] = crouch;
    p->max_frames[S_CROUCH] = f_crouch;
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

// Altera as propriedade da hitbox para que se movimente e verifica qual sprite utilizar
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key) {
    if (!a) {
        matar_pont_nulo();
    }
    
    bool andando = false;
    bool agachado = false;
    bool voando = false;

    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        a->x += a->speed_x;
        a->direction = 1; // Virado para direita
        andando =  true;

        if (a->tipo == T_ANIMAL) {
            a->min_x += a->speed_x;
            a->max_x += a->speed_x;
        }        
        a->steps++;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        a->x -= a->speed_x;
        a->direction = -1; // Virado para esquerda
        andando = true;

        if (a->tipo == T_ANIMAL) {
            a->min_x -= a->speed_x;
            a->max_x -= a->speed_x;
        }
        a->steps--;  
    }

    // Só agacha se estiver no chão
    if (al_key_down(&key, ALLEGRO_KEY_DOWN) && a->chao) {
        // Se ainda está em pé, agacha
        if (a->height == a->def_height) {
            float metade_altura = a->def_height / 2.0;
            // Ajusta o Y para que os pés continuem no chão ao encolher
            a->y += (a->def_height - metade_altura); 
            a->height = metade_altura;
        }
        agachado = true;
    } else {
        // Se soltou a tecla e está agachado (altura menor que o padrão)
        if (a->height < a->def_height) {
            // Ajusta o Y para crescer
            a->y -= (a->def_height - a->height);
            a->height = a->def_height;
            agachado = false;
        }
    }
    
    // PULO
    if (al_key_down(&key, ALLEGRO_KEY_UP) && a->chao) {
        a->speed_y += a->forca_pulo;
        a->chao = false; 
    }
    
    // VOO
    if (al_key_down(&key, ALLEGRO_KEY_SPACE) && a->fly_timer > 0) {
        a->speed_y = -5;
        a->fly_timer--;
        a->chao = false;
        voando = true;
    }

    // Recarregar combustível se estiver no chão
    if (a->chao && a->fly_timer < a->max_fly) {
        a->fly_timer += 0.1;
        if (a->fly_timer > a->max_fly) {
            a->fly_timer = a->max_fly;
        }
    }

    int novo_estado = S_IDLE;
    
    if (voando) {
         novo_estado = S_FLY;
    } else if (!a->chao) {
        novo_estado = S_JUMP;
    } else if (agachado) {
        novo_estado = S_CROUCH;
    } else if (andando) {
        novo_estado = S_WALK;
    } else{
        novo_estado = S_IDLE;
    };

    // Se mudou de estado, reseta a animação para o frame 0
    if (novo_estado != a->current_state) {
        a->current_state = novo_estado;
        a->current_frame = 0;
        a->frame_timer = 0;
    }

    // atualiza o frame
    // Incrementa o timer
    a->frame_timer++;
    if (a->frame_timer >= a->frame_delay) {
        a->frame_timer = 0;
        
        // Passa para o próximo frame
        a->current_frame++;
        
        // Se chegou no final da tira de imagens, volta para o começo (loop)
        if (a->current_frame >= a->max_frames[a->current_state]) {
            a->current_frame = 0;
        }
    }
}

void desenha_hitbox(struct hitbox *a) {
    if (!a) {
        matar_pont_nulo();
    }

    if (!a->sprites[0]) {
        return;
    }

    float offset_topo = 15.0; 
    float offset_base = 10.0;

    // alarga a imagem (borda extra)
    float ajuste_lateral = 30.0; 

    al_draw_scaled_bitmap(a->sprites[0], 0, 0, al_get_bitmap_width(a->sprites[0]), al_get_bitmap_height(a->sprites[0]), a->x - ajuste_lateral, a->y - offset_topo, a->width + ajuste_lateral * 2, a->height + offset_topo + offset_base, 0);
}

void desenha_personagem(struct hitbox *p, int flag) {
    if (!p) {
        matar_pont_nulo();
    }
    
    ALLEGRO_BITMAP *sprite_to_draw = p->sprites[p->current_state];
    
    // Se não tiver sprite usa o IDLE
    int frames_count = p->max_frames[p->current_state];
    if (!sprite_to_draw) {
        sprite_to_draw = p->sprites[S_IDLE];
        frames_count = p->max_frames[S_IDLE];
        
        if (!sprite_to_draw) {
            return;
        }
    }
    

    // calcula as dimensões de um frame
    int frame_width = al_get_bitmap_width(sprite_to_draw) / frames_count;
    int frame_height = al_get_bitmap_height(sprite_to_draw);

    int draw_flags = flag;
    if (p->direction == -1) {
        draw_flags = ALLEGRO_FLIP_HORIZONTAL;
    }

    // desenha o pedaço do frame atual
    al_draw_scaled_bitmap(sprite_to_draw, frame_width * p->current_frame, 0, frame_width, frame_height, p->x, p->y, p->width, p->height, draw_flags);
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

    obs->x += obs->mov_speed * obs->mov_direcao;

    // Verifica limites e inverte a direção
    if (obs->x <= obs->min_x) {
        obs->x = obs->min_x;
        obs->mov_direcao = 1;
    }
    else if (obs->x >= obs->max_x) {
        obs->x = obs->max_x;
        obs->mov_direcao = -1;
    }
}

// Faz o objeto se mover sozinho dentro dos limites
void atualiza_nuvem_movel(struct hitbox *obs) {
    if (!obs) {
        return;
    }

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