#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x, y; // Posição do jogador na tela
    float w, h // Altura e largura do jogador
} hitbox;

// Atribuir elementos do jogador
hitbox *criar_hitbox(float width, float height, float x_inicio, float y_inicio, int max_x, int max_y) {
    if (x_inicio + width > max_x || y_inicio + height > max_y || x_inicio < 0 || y_inicio < 0) {
        printf("Posição além do limite do display.\n");
        return NULL;
    }

    hitbox *nov_hit = (hitbox*) malloc(sizeof(hitbox));
    nov_hit->h = height;
    nov_hit->w = width;
    nov_hit->x = x_inicio;
    nov_hit->y = y_inicio;

    return nov_hit;
}

// Jogador andar para direita, esquerda, pular a abaixar
movimenta_hitbox(hitbox *hit, int steps, int trejectory, int max_x, int max_y) {
    if (condition)
    {
        /* code */
    }
    
} 
destruir_hitbox();