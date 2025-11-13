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
    struct mundo *mundo = criar_mundo();

    // Registra as fontes dos eventos buscados no programa
    al_register_event_source(mundo->fila_eventos, al_get_display_event_source(mundo->display)); // Eventos da tela
    al_register_event_source(mundo->fila_eventos, al_get_timer_event_source(mundo->timer));     // Eventos do timer (FPS)
    al_register_event_source(mundo->fila_eventos, al_get_mouse_event_source());          // Eventos do mouse

    // Esconde o cursor do mouse
    al_hide_mouse_cursor(mundo->display);

    // Inicia o timer
    al_start_timer(mundo->timer);
    bool game_done = false;

    // Loop principal do jogo
    while (!game_done) {
        int selected_option = menu(mundo);
        if (selected_option == 1) {
            // Iniciar Fase 0
            printf("Entrar na Fase 0\n");
            game_done = true;
        } else if (selected_option == 2) {
            // Terminar jogo e fechar tela
            printf("Sair do jogo.\n");
            game_done = true;
        } else {
            continue;
        }
    }
    
    // Destruir variáveis

    return 0;
}