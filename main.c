#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>

#include "erros.h"
#include "inicio.h"
#include "telas.h"

int main() {
    // Inicialização e criação de variáveis do programa
    inicializar();

    // Criação do display do jogo e definição da largura e altura da tela
    int largura, altura;
    ALLEGRO_DISPLAY *display = criar_display(&largura, &altura);
    if (!display) {
        matarProgramaErro(4);
    }
    
    // Criação da fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        al_destroy_display(display);
        matarProgramaErro(4);
    }

    // Cria um timer (relógio) do jogo para rodar a 60 FPS (frames per second)
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        al_destroy_display(display);
        al_destroy_event_queue(fila_eventos);
        matarProgramaErro(4);
    }

    // Registra as fontes dos eventos buscados no programa
    al_register_event_source(fila_eventos, al_get_display_event_source(display)); // Eventos da tela
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));     // Eventos do timer (FPS)
    al_register_event_source(fila_eventos, al_get_mouse_event_source());          // Eventos do mouse

    // Esconde o cursor do mouse
    al_hide_mouse_cursor(display);

    // Inicia o timer
    al_start_timer(timer);
    bool game_done = false;

    // Loop principal do jogo
    while (!game_done) {
        int selected_option = menu(display, fila_eventos, timer, altura, largura);
        if (selected_option == 1) {
            // Iniciar Fase 0
        } else if (selected_option == 2) {
            // Terminar jogo e fechar tela
        } else {
            continue;
        }

        
    }
    
    // Destruir variáveis

    return 0;
}