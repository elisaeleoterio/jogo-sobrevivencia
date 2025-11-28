#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h"
#include "inicio.h"
#include "obstaculos.h"
#include "hitbox.h"

#define TOLERANCIA 0.01

// faz a verificação de colisão com todos os obstáculos da lista
bool colide_obs(struct hitbox *a, struct obstacle *b) {
    if (!a) {
        matar_pont_nulo();
    }
    
    if (!b) {
        return false;
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

// Adiciona novo obstáculo na cabeça da lista
void adicionar_obstaculo(struct obstacle **raiz, struct hitbox *hitbox) {
    if (!hitbox) {
        matar_pont_nulo();
    }

    struct obstacle *novo = malloc(sizeof(struct obstacle));
    if (!novo) {
        matar_falta_memoria();
    }

    novo->hitbox = hitbox;
    novo->next = *raiz;
    *raiz = novo;
}

void destruir_lista_obstaculos(struct obstacle *raiz) {
    if (!raiz) {
        return;
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
    if (!raiz) {
        return;
    }

    struct obstacle *atual = raiz;
    while (atual != NULL) {
        movimenta_hitbox(atual->hitbox, key);
        atual = atual->next;
    }
}

void salva_pos_anterior_lista(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }

    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atual->hitbox->old_x = atual->hitbox->x;
        
        if (atual->hitbox->tipo == T_ANIMAL) {
            atual->hitbox->old_min_x = atual->hitbox->min_x;
            atual->hitbox->old_max_x = atual->hitbox->max_x;
        } else if (atual->hitbox->tipo == T_NUVEM_MOVEL) {
            atual->hitbox->old_min_y = atual->hitbox->min_y;
            atual->hitbox->old_max_y = atual->hitbox->max_y;
        }
        
        atual = atual->next;
    }
}

void reverte_pos_lista(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }

    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atual->hitbox->x = atual->hitbox->old_x;
        if (atual->hitbox->tipo == T_ANIMAL) {
            atual->hitbox->min_x = atual->hitbox->old_min_x;
            atual->hitbox->max_x = atual->hitbox->old_max_x;
        } else if (atual->hitbox->tipo == T_NUVEM_MOVEL) {
            atual->hitbox->min_y = atual->hitbox->old_min_y;
            atual->hitbox->max_y = atual->hitbox->old_max_y;
        }
        
        atual = atual->next;
    }
}

void desenha_lista_obst(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }
    
    struct obstacle *atual = raiz;
    while (atual) {
        struct hitbox *hit = atual->hitbox;
        if (hit->tipo == T_BURACO && !hit->active) {
            atual=atual->next;
            continue;
        }
        desenha_hitbox(hit);
        atual = atual->next;
    }
}

// Verifica colisão no eixo x para os obstáculos
// Retorna true se o mundo parou e false se o mundo continua
bool verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key) {
    if (!player) {
        matar_pont_nulo();
    }

    if (!lista_obstaculos) {
        return false;
    }
    
    // Variável para verificar reversão
    bool parou = false;

    struct obstacle *atual = lista_obstaculos;
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        
        if (verifica_colisao(player, obstacle)) {
            // Se for um buraco e estiver desativado, ignora colisão lateral
            if (obstacle->tipo == T_BURACO && !obstacle->active) {
                atual = atual->next;
                continue;
            }
            
            // Verifica se no frame anterior já estavam sobrepostos no eixo X
            bool x_sobrepor = (player->old_x + player->width > obstacle->old_x + TOLERANCIA) && 
                              (player->old_x < obstacle->old_x + obstacle->width - TOLERANCIA);
                            
            if (!x_sobrepor) {
                // Veio da esquerda
                if (player->old_x + player->width <= obstacle->old_x + TOLERANCIA) {
                    // Se está indo para a direita, para o mundo
                    if (al_key_down(&key, ALLEGRO_KEY_RIGHT) && !parou) {
                        reverte_pos_lista(lista_obstaculos);
                        *back_x = old_back_x;
                        parou = true;
                        
                        // Ajusta player (se o mundo reverteu, obstacle->x volta a ser prev_x)
                        player->x = obstacle->x - player->width - 0.1;
                    }                 
                }  
                // Veio da direita
                else {
                    if (al_key_down(&key, ALLEGRO_KEY_LEFT) && !parou) {
                        reverte_pos_lista(lista_obstaculos);
                        *back_x = old_back_x;
                        parou = true;
                        player->x = obstacle->x + obstacle->width + 0.1;
                    }                   
                }
            }
        }
        atual = atual->next;
    }
    return parou;
}

void verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos) {
    if (!player) {
        matar_pont_nulo();
    }
    
    if (!lista_obstaculos) {
        return;
    }
    
    struct obstacle *atual = lista_obstaculos;
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        if (verifica_colisao(player, obstacle)) {
            if (atual->hitbox->tipo == T_BURACO && !atual->hitbox->active) {
                atual = atual->next;
                continue;
            } else {
                // jogador caindo
                if (player->speed_y > 0) { 
                    player->y = obstacle->y - player->height;
                    player->speed_y = 0;
                    player->chao = true;
                // jogador pulou muito perto (bate a cabeça)
                } else if (player->speed_y < 0) {
                    player->y = obstacle->y + obstacle->height;
                    player->speed_y = 0;
                }
            }
        }
        atual = atual->next;
    }
}


void atualiza_lista_buracos(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }

    struct obstacle *atual = raiz;
    while (atual != NULL) {
        struct hitbox *obs = atual->hitbox;
        obs->timer++;
        if (obs->timer >= obs->interval) {
                obs->timer = 0;
                // Alterna entre aberto (false) e fechado (true)
                obs->active = !obs->active; 
        }
        atual = atual->next;
    }
}

void atualiza_lista_animais(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }
    
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atualiza_animal(atual->hitbox);
        atual = atual->next;
    }
}

void atualiza_lista_nuvem_movel(struct obstacle *raiz) {
    if (!raiz) {
        return;
    }

    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atualiza_nuvem_movel(atual->hitbox);
        atual = atual->next;
    }
}
