#include <stdint.h>
#include <stdio.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "hitbox.h"

struct hitbox *cria_hitbox(float x, float y, float w, float h, float speed, const char *filename) {
    struct hitbox *hit = malloc(sizeof(struct hitbox));
    if (!hit) {
        matarProgramaErro(5);
    }

    hit->x = x;
    hit->y = y;

    hit->width = w;
    hit->height = h;

    hit->speed = speed;

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

    // a está totalmente abaixo de b
    if (a->y + a->height < b->y) {
        return 0;
    }

    // a está totalmente a direita de b
    if (a->x < b->x + b->width) {
        return 0;
    }
    // a está totalmente acima de b
    if (a->y < b->y + b->height) {
        return 0;
    }
    
    // Há colisão
    return 1;    
}

// Altera as propriedade da hitbox para que se movimente com teclados
void movimenta_hitbox(struct hitbox **a, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        (*a)->x += (*a)->speed;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        (*a)->x -= (*a)->speed;  
    }
    if (al_key_down(&key, ALLEGRO_KEY_DOWN)) {
        (*a)->y += (*a)->speed; 
    }
    if (al_key_down(&key, ALLEGRO_KEY_UP)) {
        (*a)->y -= (*a)->speed; 
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