#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h"
#include "inicio.h"

#define TOLERANCIA 0.05
#define PLATAFORMA 1
#define ESPINHO 2
#define ANIMAL 3
#define BURACO_FIXO 4
#define BURACO_SURGE 5
#define PLATAFORMA_MOVEL 6
#define CAVERNA 7
#define PERSONAGEM 8

struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed_x, float speed_y, float forca_pulo, const char *filename, int tipo) {
    struct hitbox *hit = malloc(sizeof(struct hitbox));
    if (!hit) {
        matarProgramaErro(5);
    }

    hit->x = x;
    hit->y = y;

    hit->width = w;
    hit->height = h;

    hit->speed_x = speed_x;
    hit->speed_y = speed_y;

    hit->forca_pulo = forca_pulo;

    hit->chao = true;
    hit->steps = 0;

    hit->life = 5;
    hit->tipo = tipo;
    hit->old_x = x;

    hit->patrol_speed = 0;
    hit->min_x = 0;
    hit->max_x = 0;
    hit->patrol_dir = 1;
    
    if (filename) {
        hit->sprite = al_load_bitmap(filename);
        if (!hit->sprite) {
            matarProgramaErro(4);
        }
    } else {
        hit->sprite = NULL;
    }
    
    return hit;
}


// Retorna 1 se houver colisão e 0 se não houver
int verifica_colisao(struct hitbox *a, struct hitbox *b) {
    if (!a || !b) {
        matarProgramaErro(3);
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
    
    // Há colisão
    return 1;    
}

bool colide_obs(struct hitbox *a, struct obstacle *b) {
    if (!a || !b) {
        matarProgramaErro(3);
    }

    struct obstacle *atual = b;
    while (atual != NULL) {
        if (verifica_colisao(a, atual->hitbox)) {
            return true;
        }
        
        atual = atual->next;
    }

    return false;
}

// Altera as propriedade da hitbox para que se movimente com teclados com gravidade
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        a->x += a->speed_x;

        if (a->tipo == ANIMAL) {
            a->min_x += a->speed_x;
            a->max_x += a->speed_x;
        }        
        a->steps++;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        a->x -= a->speed_x;

        if (a->tipo == ANIMAL) {
            a->min_x -= a->speed_x;
            a->max_x -= a->speed_x;
        }
        a->steps--;  
    }
    if (al_key_down(&key, ALLEGRO_KEY_DOWN)) {
        // Mudar a imagem do sprite para abaixar
        // Mudar temporariamente a altura do hitbox
    }
    if (al_key_down(&key, ALLEGRO_KEY_UP) && a->chao) {
        a->speed_y += a->forca_pulo;
        a->chao = false; 
    }
}

void desenha_hitbox(struct hitbox *a, float dest_x, float dest_y, 
                    float larg_final, float alt_final, int flag) {
    if (!a) {
        matarProgramaErro(2);
    }

    if (!a->sprite) {
        return;
    }
    
    al_draw_scaled_bitmap(a->sprite, a->x, a->y, a->width, a->height, 
                        dest_x, dest_y, larg_final, alt_final, flag);
    
}

void destruir_hitbox(struct hitbox *a) {
    if (!a) {
        matarProgramaErro(2);
    }
    if (a->sprite) {
        al_destroy_bitmap(a->sprite);
    }
    free(a);
    
}

// Adiciona novo obstáculo na cabeça da lista
void adicionar_obstaculo(struct obstacle **raiz, struct hitbox *hitbox) {
    if (!hitbox) {
        matarProgramaErro(2); // REVER CÒDIGO DE ERRO
    }

    struct obstacle *novo = malloc(sizeof(struct obstacle));
    if (!novo) {
        matarProgramaErro(5); // REVER CÒDIGO DE ERRO
    }

    novo->hitbox = hitbox;
    novo->next = *raiz;
    *raiz = novo;
}

void destruir_lista_obstaculos(struct obstacle *raiz) {
    if (!raiz) {
        matarProgramaErro(2);
    }

    struct obstacle *atual = raiz;
    while (atual) {
        struct obstacle *prox = atual->next;
        destruir_hitbox(atual->hitbox);
        free(atual);
        atual = prox;
    }
}

void movimenta_lista_obstaculos(struct obstacle *raiz, ALLEGRO_KEYBOARD_STATE key) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        movimenta_hitbox(atual->hitbox, key);
        atual = atual->next;
    }
}

void desenha_lista_obstaculos(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        desenha_hitbox(atual->hitbox, 0, 0, atual->hitbox->width, atual->hitbox->height, 0);
        atual = atual->next;
    }
}

void salva_pos_anterior_lista(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atual->hitbox->old_x = atual->hitbox->x;
        if (atual->hitbox->tipo == ANIMAL) {
            atual->hitbox->old_min_x = atual->hitbox->min_x;
            atual->hitbox->old_max_x = atual->hitbox->max_x;
        } else if (atual->hitbox->tipo == PLATAFORMA_MOVEL) {
            atual->hitbox->old_min_y = atual->hitbox->min_y;
            atual->hitbox->old_max_y = atual->hitbox->max_y;
        }
        
        atual = atual->next;
    }
}

void reverte_pos_lista(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atual->hitbox->x = atual->hitbox->old_x;
        if (atual->hitbox->tipo == ANIMAL) {
            atual->hitbox->min_x = atual->hitbox->old_min_x;
            atual->hitbox->max_x = atual->hitbox->old_max_x;
        } else if (atual->hitbox->tipo == PLATAFORMA_MOVEL) {
            atual->hitbox->min_y = atual->hitbox->old_min_y;
            atual->hitbox->max_y = atual->hitbox->old_max_y;
        }
        
        atual = atual->next;
    }
}

// Verifica Colisão no Eixo X para TODOS os obstáculos -> Retorna true se o mundo parou e false se o mundo continua
bool verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key) {
    if (!player || !lista_obstaculos) {
        matarProgramaErro(2);
    }
    
    // Variável para verificar reversão
    bool world_stopped = false; 
    struct obstacle *atual = lista_obstaculos;
    
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        
        if (verifica_colisao(player, obstacle)) {
            // Se for um BURACO (FIXO ou SURGE), ignoramos colisão lateral
            if (obstacle->tipo == BURACO_FIXO || obstacle->tipo == BURACO_SURGE) {
                atual = atual->next;
                continue;
            }
            
            // Verifica se no frame anterior eles JÁ estavam sobrepostos no eixo X
            bool was_x_overlap = (player->old_x + player->width > obstacle->old_x + TOLERANCIA) && 
                                (player->old_x < obstacle->old_x + obstacle->width - TOLERANCIA);
                            
            // Só resolve colisão X se ANTES eles estavam separados
            if (!was_x_overlap) {
                // Veio da ESQUERDA
                if (player->old_x + player->width <= obstacle->old_x + TOLERANCIA) {
                    // Se pressionando direita, para o mundo
                    if (al_key_down(&key, ALLEGRO_KEY_RIGHT) && !world_stopped) {
                        if (!world_stopped) {
                            reverte_pos_lista(lista_obstaculos);
                            *back_x = old_back_x;
                            world_stopped = true;
                        }
                        // Ajusta player (se o mundo reverteu, obstacle->x volta a ser prev_x)
                        player->x = obstacle->x - player->width - 0.1;
                    }
                                        
                }  
                // Veio da DIREITA
                else {
                    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
                        if (!world_stopped) {
                            reverte_pos_lista(lista_obstaculos);
                            *back_x = old_back_x;
                            world_stopped = true;
                        }
                        player->x = obstacle->x + obstacle->width + 0.1;
                    }
                                    
                }
            }
        }
        atual = atual->next;
    }
    return world_stopped;
}

bool verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos) {
    if (!player || !lista_obstaculos) {
        matarProgramaErro(2);
    }

    bool in_hole = false;
    struct obstacle *atual = lista_obstaculos;
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        if (verifica_colisao(player, obstacle)) {
            if (atual->hitbox->tipo == BURACO_FIXO) {
                in_hole = true;
            } else if (atual->hitbox->tipo == BURACO_SURGE){
                if (atual->hitbox->active){
                    in_hole = true;
                }
            } else {
                if (player->speed_y > 0) { // Caindo
                    player->y = obstacle->y - player->height;
                    player->speed_y = 0;
                    player->chao = true;
                } else if (player->speed_y < 0) { // Batendo cabeça
                    player->y = obstacle->y + obstacle->height;
                    player->speed_y = 0;
                }
            }
        }
        atual = atual->next;
    }
    return in_hole;
}

// Configura a patrulha
void configura_patrulha(struct hitbox *obs, float distancia_patrulha, float velocidade_propria) {
    if (!obs) {
        matarProgramaErro(3);
    }

    obs->min_x = obs->x;
    obs->max_x = obs->x + distancia_patrulha;
    obs->patrol_speed = velocidade_propria;
    obs->patrol_dir = 1; // Começa indo para a direita
}

// Faz o objeto andar sozinho dentro dos limites
void atualiza_patrulha(struct hitbox *obs) {
    if (!obs) {
        matarProgramaErro(3);
    }

    // Move baseado na velocidade própria e direção atual
    obs->x += obs->patrol_speed * obs->patrol_dir;

    // Verifica limites e inverte a direção
    if (obs->x <= obs->min_x) {
        obs->x = obs->min_x;
        obs->patrol_dir = 1; // Vira para direita
    }
    else if (obs->x >= obs->max_x) {
        obs->x = obs->max_x;
        obs->patrol_dir = -1; // Vira para esquerda
    }
}

void atualiza_lista_patrulha(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atualiza_patrulha(atual->hitbox);
        atual = atual->next;
    }
}


void configura_buraco_periodico(struct hitbox *obs, int intervalo_frames) {
    if (!obs) return;
    obs->tipo = BURACO_SURGE;
    obs->active = true; // Começa aberto
    obs->timer = 0;
    obs->interval = intervalo_frames;
}

void atualiza_buracos_periodicos(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        struct hitbox *obs = atual->hitbox;
        if (obs->tipo == BURACO_SURGE) {
            obs->timer++;
            if (obs->timer >= obs->interval) {
                obs->timer = 0;
                obs->active = !obs->active; // Alterna entre aberto (true) e fechado (false)
            }
        }
        atual = atual->next;
    }
}


// Configura a patrulha
void configura_plat_movel(struct hitbox *obs, float distancia_altura, float velocidade_propria) {
    if (!obs) {
        matarProgramaErro(3);
    }

    obs->min_y = obs->y;
    obs->max_y = obs->y + distancia_altura;
    obs->patrol_speed = velocidade_propria;
    obs->patrol_dir = 1; // Começa indo para a direita
}

// Faz o objeto se mover sozinho dentro dos limites
void atualiza_plataforma(struct hitbox *obs) {
    if (!obs) {
        matarProgramaErro(3);
    }

    // Move baseado na velocidade própria e direção atual
    obs->y += obs->patrol_speed * obs->patrol_dir;

    // Verifica limites e inverte a direção
    if (obs->y <= obs->min_y) {
        obs->y = obs->min_y;
        obs->patrol_dir = 1; // Vira para cima
    }
    else if (obs->y >= obs->max_y) {
        obs->y = obs->max_y;
        obs->patrol_dir = -1; // Vira para baixo
    }
}

void atualiza_lista_plat_movel(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atualiza_plataforma(atual->hitbox);
        atual = atual->next;
    }
}