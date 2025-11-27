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
#include "hitbox.h"
#include "menu.h"

int game_over(struct mundo *mundo) {
    if (!mundo) {
        matar_pont_nulo();
    }
    
    // Carrega imagem de fundo
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/menu_background.jpg");
    if (!background) {
        matarProgramaErro(4);
    }

    // Carrega imagem que irá substituir o cursor do mouse
    ALLEGRO_BITMAP *mouse_cursor = al_load_bitmap("assets/images/mouse_cursor.png");
    if (!mouse_cursor) {
        al_destroy_bitmap(background);
        matarProgramaErro(4);
    }

    // Carrega a fonte que será utilizada
    ALLEGRO_FONT *font_go = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 60, 0);
    if (!font_go) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        matarProgramaErro(4);
    }
    ALLEGRO_FONT *font_button = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 50, 0);
    if (!font_button) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        matarProgramaErro(4);
    }

    // Variáveis de estado do menu
    bool go_done = false;
    // Flag para controlar quando desenhar na tela
    bool redraw = true; 

    // Variáveis de estado do mouse e seleção
    int mouse_x = 0;
    int mouse_y = 0;

    // 0 = nenhum, 1 = Restart, 2 = Exit
    int selected_option = 0; 

    // Loop principal do menu
    while (!go_done) {
        
        // Armazena a altura a font
        int font_height = al_get_font_line_height(font_go);

        // Ponto X central
        float go_x = mundo->largura / 2.0; 
        // Ponto Y central
        float go_y = mundo->altura / 4.0;

        // Espaçamento entre os itens
        float padding = font_height * 1; // Meia linha de espaço

        // Posições Y para desenhar o texto (al_draw_text usa o Y do topo)
        float title_y = menu_y;
        float start_y = menu_y + padding;
        float exit_y = start_y + padding; 

        ALLEGRO_EVENT evento;
        // Espera por um evento (do usuário ou do timer)
        al_wait_for_event(mundo->fila_eventos, &evento);

        switch (evento.type) {
            // Realiza esse evento a cada tique do timer - Verificação da seleção das opções do menu
            case ALLEGRO_EVENT_TIMER: 
                // Obter as larguras dos textos
                float start_text_width = al_get_text_width(font_button, "START");
                float exit_text_width = al_get_text_width(font_button, "EXIT");

                // Definir as áreas das hitboxes
                // BOTÃO START
                float start_x1 = menu_x - (start_text_width / 2.0);
                float start_x2 = menu_x + (start_text_width / 2.0);
                float start_y1 = start_y; // Y do topo (usado no al_draw_text)
                float start_y2 = start_y + font_height; // Y da base

                // BOTÃO EXIT
                float exit_x1 = menu_x - (exit_text_width / 2.0);
                float exit_x2 = menu_x + (exit_text_width / 2.0);
                float exit_y1 = exit_y; // Y do topo
                float exit_y2 = exit_y + font_height; // Y da base
            
                // Verificação se o mouse está encima de algum botão (muda a cor)
                if ((mouse_x >= start_x1 && mouse_x <= start_x2) && (mouse_y >= start_y1 && mouse_y <= start_y2)) {
                    selected_option = 1;
                } else if ((mouse_x >= exit_x1 && mouse_x <= exit_x2) && (mouse_y >= exit_y1 && mouse_y <= exit_y2)) {
                    selected_option = 2;
                } else {
                    selected_option = 0;
                }
                redraw = true;
                break;

            // Realiza esse evento a cada movimentação do mouse
            case ALLEGRO_EVENT_MOUSE_AXES:
                // Atualização da posição do mouse
                mouse_x = evento.mouse.x;
                mouse_y = evento.mouse.y;
                break;

            // Realiza esse evento se o usuário selecionar alguma opçao do menu
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (selected_option == 1) {
                    printf("Botão START pressionado.\n");
                    menu_done = true;
                } else if (selected_option == 2) {
                    printf("Botão EXIT selecionado.\n");
                    menu_done = true;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                menu_done = true;
                break;
            default:
                break;
        }

        if (redraw && al_is_event_queue_empty(mundo->fila_eventos)) {
            redraw = false;

            // Definir cores
            ALLEGRO_COLOR fundo = al_map_rgb(14, 17, 22);
            ALLEGRO_COLOR cor_normal = al_map_rgb(255, 185, 210); // Blush Pop
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            
            // Reseta para a cor de fundo
            al_clear_to_color(fundo);

            // Desenha a imagem de fundo
            int background_width = al_get_bitmap_width(background);
            int background_heitgh = al_get_bitmap_height(background);
            al_draw_scaled_bitmap(background, 0, 0, background_width, background_heitgh, 0, 0, mundo->largura, mundo->altura, ALLEGRO_MAG_LINEAR);
            
            // Título
            al_draw_text(font_menu, cor_normal, menu_x, title_y, ALLEGRO_ALIGN_CENTER, "MENU");
            // START
            al_draw_text(font_button, (selected_option == 1) ? cor_selecionado : cor_normal, menu_x, start_y, ALLEGRO_ALIGN_CENTER, "start");
            // EXIT
            al_draw_text(font_button, (selected_option == 2) ? cor_selecionado : cor_normal, menu_x, exit_y, ALLEGRO_ALIGN_CENTER, "exit");

            // Desenhar mouse falso
            int mouse_width = al_get_bitmap_width(mouse_cursor);
            int mouse_height = al_get_bitmap_height(mouse_cursor);
            al_draw_scaled_bitmap(mouse_cursor, 0, 0, mouse_width, mouse_height, mouse_x, mouse_y, 
                                    mouse_width * 0.02, mouse_height * 0.02, ALLEGRO_MIN_LINEAR);     
            
            // Copies or updates the front and back buffers so that what has been drawn 
            // previously on the currently selected display becomes visible on screen. 
            al_flip_display();
        }
    }

    // Destroi variáveis para finalizar o programa
    al_destroy_bitmap(mouse_cursor);
    al_destroy_bitmap(background);
    al_destroy_font(font_button);
    al_destroy_font(font_menu);

    return selected_option;
}