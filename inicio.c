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
        matarProgramaErro(5);
    }

    mundo->display = criar_display(&mundo->largura, &mundo->altura);
    if (!mundo->display) {
        matarProgramaErro(4);
    }
    
    mundo->timer = al_create_timer(1.0 / 60.0); // Roda em 60 FPS
    if (!mundo->timer) {
        al_destroy_display(mundo->display);
        matarProgramaErro(4);
    }

    mundo->fila_eventos = al_create_event_queue();
    if (!mundo->fila_eventos) {
        al_destroy_display(mundo->display);
        al_destroy_timer(mundo->timer);
        matarProgramaErro(4);
    }

    mundo->gravidade = GRAVIDADE;
    mundo->chao_y = mundo->altura - 200;
    
    return mundo;

}

void destruir_mundo(struct mundo *mundo) {
    if (!mundo) {
        matarProgramaErro(3);
    }

    al_destroy_display(mundo->display);
    al_destroy_event_queue(mundo->fila_eventos);
    al_destroy_timer(mundo->timer);
    
}

// FUNÇÕES DE INICIALIZAÇÃO DO JOGO E DAS TELAS (ADDONS E VARIÁVEIS)

void inicializar() {
    if (!al_init()) {
        matarProgramaErro(1);
    }

    if (!al_init_image_addon()) {
        matarProgramaErro(1);
    }

    if (!al_init_font_addon()) {
        matarProgramaErro(1);
    }

    if (!al_init_ttf_addon())
    {
        matarProgramaErro(1);
    }

    if (!al_init_primitives_addon()) {
        matarProgramaErro(1);
    }
    

    if (!al_install_mouse()) {
        matarProgramaErro(1);
    }

    if (!al_install_keyboard()) {
        matarProgramaErro(1);
    }
    
}

ALLEGRO_DISPLAY *criar_display(int *largura, int *altura) {
    if (!largura || !altura) {
        matarProgramaErro(3);
    }
    
    // Describes a monitor’s size and position relative to other monitors. 
    // x1, y1 will be 0, 0 on the primary display. 
    // Other monitors can have negative values if they are to the left or above the primary display. 
    // x2, y2 are the coordinates one beyond the bottom right pixel, so that x2-x1 gives 
    // the width and y2-y1 gives the height of the display.

    // Pegar a informação do monitor em que o jogo será rodado
    // 0 indica o monitor principal
    ALLEGRO_MONITOR_INFO monitor_info;
    if (!al_get_monitor_info(0, &monitor_info)) {
        matarProgramaErro(2);
    }

    // Definir a altura e largura do monitor
    *largura = monitor_info.x2 - monitor_info.x1;
    *altura = monitor_info.y2 - monitor_info.y1;

    // Setar a flag de FULLSCREEN_WINDOW - faz com que consuma a tela toda, tirando as bordas
    al_set_new_display_flags(ALLEGRO_FULLSCREEN); // OU ALLLEGRO_FULSCREEN_WINDOW

    ALLEGRO_DISPLAY *display = al_create_display(*largura, *altura);
    if (!display) {
        matarProgramaErro(4);
    }

    return display;
}

void desenha_background(ALLEGRO_BITMAP *background, float back_x) {
    if (!background) {
        matarProgramaErro(3);
    }
    
    
    int bg_w = al_get_bitmap_width(background);
    int bg_h = al_get_bitmap_height(background);
            
            
    float offset_x = fmod(back_x, bg_w);
    // float draw_y = 320 - bg_h;
            
    // Desenha a imagem de fundo
    // sx, sy (Pegar a imagem toda)
    // sw, sh (A imagem toda)
    // dx, dy (Posição na tela)
    // dw, dh (LARGURA original, ALTURA da tela)
    al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x, 0, bg_w, 320, 0);
                
    // Desenha a segunda cópia (a que "dá a volta")
    // Ela é desenhada exatamente à direita da primeira,
    // preenchendo o espaço vazio que aparece durante o scroll.
    al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w, 0, bg_w, 320, 0);
}

void movimenta_background(float *back_x, float velocidade, ALLEGRO_KEYBOARD_STATE key) {
    if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
        *back_x += velocidade;   
    }
    if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
        *back_x -= velocidade;   
    }
}