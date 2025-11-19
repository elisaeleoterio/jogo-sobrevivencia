#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h"
#include "inicio.h"

#define TOLERANCIA 0.2

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

// Altera as propriedade da hitbox para que se movimente com teclados com gravidade
void movimenta_hitbox(struct hitbox *a, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        a->x += a->speed_x;
        a->steps++;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        a->x -= a->speed_x;
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
        atual = atual->next;
    }
}

void reverte_pos_lista(struct obstacle *raiz) {
    struct obstacle *atual = raiz;
    while (atual != NULL) {
        atual->hitbox->x = atual->hitbox->old_x;
        atual = atual->next;
    }
}

// Verifica Colisão no Eixo X para TODOS os obstáculos
void verifica_colisao_obs_eixo_x(struct hitbox *player, struct obstacle *lista_obstaculos, float *back_x, float old_back_x, ALLEGRO_KEYBOARD_STATE key) {
    if (!player || !lista_obstaculos) {
        matarProgramaErro(2);
    }
    
    // Variável para verificar reversão
    bool world_stopped = false; 
    struct obstacle *atual = lista_obstaculos;
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        if (verifica_colisao(player, obstacle)) {
                        
            // Verifica se no frame anterior eles JÁ estavam sobrepostos no eixo X
            bool was_x_overlap = (player->old_x + player->width > obstacle->old_x + TOLERANCIA) && 
                                (player->old_x < obstacle->old_x + obstacle->width - TOLERANCIA);
                            
            // Só resolve colisão X se ANTES eles estavam separados
            if (!was_x_overlap) {
                // Veio da ESQUERDA
                if (player->old_x + player->width <= obstacle->old_x + TOLERANCIA) {
                    // Se pressionando direita, para o mundo
                    if (al_key_down(&key, ALLEGRO_KEY_RIGHT) && !world_stopped) {
                        reverte_pos_lista(lista_obstaculos);
                        *back_x = old_back_x;
                        world_stopped = true;
                    }
                                        
                    // Ajusta player (se o mundo reverteu, obstacle->x volta a ser prev_x)
                    player->x = obstacle->x - player->width - 0.1;
                }  
                // Veio da DIREITA
                else {
                    if (al_key_down(&key, ALLEGRO_KEY_LEFT) && !world_stopped) {
                        reverte_pos_lista(lista_obstaculos);
                        *back_x = old_back_x;
                        world_stopped = true;
                    }
                                    
                    player->x = obstacle->x + obstacle->width + 0.1;
                }
            }
        }
        atual = atual->next;
    }
}

void verifica_colisao_obs_eixo_y(struct hitbox *player, struct obstacle *lista_obstaculos) {
    if (!player || !lista_obstaculos) {
        matarProgramaErro(2);
    }

    struct obstacle *atual = lista_obstaculos;
    while (atual != NULL) {
        struct hitbox *obstacle = atual->hitbox;
        if (verifica_colisao(player, obstacle)) {
            if (player->speed_y > 0) { // Caindo
                player->y = obstacle->y - player->height;
                player->speed_y = 0;
                player->chao = true;
            } else if (player->speed_y < 0) { // Batendo cabeça
                player->y = obstacle->y + obstacle->height;
                player->speed_y = 0;
            }
        }
        atual = atual->next;
    }
    
    
}