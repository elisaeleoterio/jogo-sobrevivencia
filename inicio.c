#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "erros.h"
#include "inicio.h"

#define GRAVIDADE 0.5


// Attibuir variáveis ao mundo
struct mundo *criar_mundo() {
    struct mundo *mundo = malloc(sizeof(struct mundo));
    if (!mundo) {
        matar_falta_memoria();
    }

    mundo->display = criar_display(&mundo->largura, &mundo->altura);
    if (!mundo->display) {
        matar_falta_memoria();
    }
    
    mundo->timer = al_create_timer(1.0 / 60.0); // Roda em 60 FPS
    if (!mundo->timer) {
        al_destroy_display(mundo->display);
        matar_falta_memoria();
    }

    mundo->fila_eventos = al_create_event_queue();
    if (!mundo->fila_eventos) {
        al_destroy_display(mundo->display);
        al_destroy_timer(mundo->timer);
        matar_falta_memoria();
    }

    mundo->gravidade = GRAVIDADE;
    mundo->chao_y = mundo->altura - 200;
    
    return mundo;

}

void destruir_mundo(struct mundo *mundo) {
    if (!mundo) {
        matar_pont_nulo();
    }

    al_destroy_display(mundo->display);
    al_destroy_event_queue(mundo->fila_eventos);
    al_destroy_timer(mundo->timer);
}

// FUNÇÕES DE INICIALIZAÇÃO DO JOGO E DAS TELAS (ADDONS E VARIÁVEIS)

void inicializar() {
    if (!al_init()) {
        matar_erro_incializar();
    }

    if (!al_init_image_addon()) {
        matar_erro_incializar();
    }

    if (!al_init_font_addon()) {
        matar_erro_incializar();
    }

    if (!al_init_ttf_addon()) {
        matar_erro_incializar();
    }

    if (!al_init_primitives_addon()) {
        matar_erro_incializar();
    }
    

    if (!al_install_mouse()) {
        matar_erro_incializar();
    }

    if (!al_install_keyboard()) {
        matar_erro_incializar();
    }
    
}

ALLEGRO_DISPLAY *criar_display(int *largura, int *altura) {
    if (!largura || !altura) {
        matar_pont_nulo();
    }
    
    // Pegar a informação do monitor em que o jogo será rodado
    // 0 indica o monitor principal
    ALLEGRO_MONITOR_INFO monitor_info;
    if (!al_get_monitor_info(0, &monitor_info)) {
        matar_falta_memoria();
    }

    // Definir a altura e largura do monitor
    *largura = monitor_info.x2 - monitor_info.x1;
    *altura = monitor_info.y2 - monitor_info.y1;

    // Setar a flag de FULLSCREEN_WINDOW - faz com que consuma a tela toda, tirando as bordas
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // OU ALLLEGRO_FULSCREEN_WINDOW

    ALLEGRO_DISPLAY *display = al_create_display(*largura, *altura);
    if (!display) {
        matar_falta_memoria();
    }

    return display;
}

void movimenta_background(float *back_x, float velocidade, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        *back_x += velocidade;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        *back_x -= velocidade;   
    }
}