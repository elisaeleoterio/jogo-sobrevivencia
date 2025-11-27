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
#include "fase_zero.h"
#include "hitbox.h"
#include "menu.h"

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

    bool game_done = false;
    
    // Inicia o timer
    al_start_timer(mundo->timer);
    
    // Loop principal do jogo
    while (!game_done) {
        // Roda o menu e verifica a escolha do usuário
        int selected_option = screen(mundo, "MENU", NULL, "Start", "Exit");
        
        // Selecionou a opção START
        if (selected_option == 1) {
            // Loop da Fase (Permite reiniciar sem voltar ao menu principal)
            bool fase = true;
            while (fase) {
                int resultado = fase_zero(mundo);
                
                // 1 = Vitória
                if (resultado == 0) {
                    int end_option = screen(mundo, "Parabéns!", "Missão cumprida!", "Restart", "Exit");
                    
                    if (end_option == 2) { // Exit
                        fase = false;
                        game_done = true;
                    }
                    // Se end_option == 1 (Restart), o loop while(fase_rodando) continua e chama fase_zero novamente
                } 
                // 0 = Derrota (Game Over)
                else if (resultado == 1) {
                    int end_option = screen(mundo, "Game Over", "Tente novamente!", "Restart", "Exit");
                    
                    if (end_option == 2) { // Exit
                        fase = false;
                        game_done = true;
                    }
                }
            }
            
            
        // Selecionou a opção EXIT
        } else if (selected_option == 2) {
            // Terminar jogo e fechar tela
            printf("Sair do jogo.\n");
            game_done = true;
        } else {
            printf("Rodou dnv.\n");
            // Rodar o menu até ter alguma seleção
            continue;
        }
    }
    
    // Destruir variáveis
    destruir_mundo(mundo);

    return 0;
}