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
#include "screens.h"

int screen(struct mundo *mundo, char *title, char *subtitle, char *button1, char *button2) {
    if (!mundo) {
        matar_pont_nulo();
    }
    
    // Carrega imagem de fundo do menu
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/menu_background.jpg");
    if (!background) {
        matar_falta_memoria();
    }

    // Carrega imagem que irá substituir o cursor do mouse
    ALLEGRO_BITMAP *mouse_cursor = al_load_bitmap("assets/images/mouse_cursor.png");
    if (!mouse_cursor) {
        al_destroy_bitmap(background);
        matar_falta_memoria();
    }

    // Carrega a fonte que será utilizada
    ALLEGRO_FONT *font_title = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 70, 0);
    if (!font_title) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        matar_falta_memoria();
    }
    
    ALLEGRO_FONT *font_subtitle = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 35, 0);
    if (!font_subtitle) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        al_destroy_font(font_title);
        matar_falta_memoria();
    }
    
    ALLEGRO_FONT *font_button = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 50, 0);
    if (!font_button) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        al_destroy_font(font_title);
        al_destroy_font(font_subtitle);
        matar_falta_memoria();
    }

    // Variáveis de estado do menu
    bool screen_done = false;
    bool redraw = true; 

    // Variáveis de estado do mouse e seleção
    int mouse_x = 0;
    int mouse_y = 0;

    // 0 = nenhum, 1 = Start, 2 = Exit
    int selected_option = 0; 

    // Loop principal do menu
    while (!screen_done) {
        
        // Armazena a altura a font
        int font_height = al_get_font_line_height(font_title);

        // Ponto X central
        float screen_x = mundo->largura / 2.0; 
        // Ponto Y central
        float screen_y = mundo->altura / 4.0;

        // Espaçamento entre os itens
        float padding = font_height * 0.5; // Meia linha de espaço

        // Posições Y para desenhar o texto (al_draw_text usa o Y do topo)
        float title_y = screen_y;
        float subtitle_y = screen_y + padding;
        float bt1_y = screen_y + 2*padding;
        float bt2_y = bt1_y + 2*padding; 

        ALLEGRO_EVENT evento;
        // Espera por um evento (do usuário ou do timer)
        al_wait_for_event(mundo->fila_eventos, &evento);

        switch (evento.type) {
            // Realiza esse evento a cada tique do timer - Verificação da seleção das opções do menu
            case ALLEGRO_EVENT_TIMER: 
                // Obter as larguras dos textos
                float bt1_text_width = al_get_text_width(font_button, button1);
                float bt2_text_width = al_get_text_width(font_button, button2);

                // Definir as áreas das hitboxes
                // BOTÃO 1
                float bt1_x1 = screen_x - (bt1_text_width / 2.0);
                float bt1_x2 = screen_x + (bt1_text_width / 2.0);
                float bt1_y1 = bt1_y; // Y do topo (usado no al_draw_text)
                float bt1_y2 = bt1_y + font_height; // Y da base

                // BOTÃO 2
                float bt2_x1 = screen_x - (bt2_text_width / 2.0);
                float bt2_x2 = screen_x + (bt2_text_width / 2.0);
                float bt2_y1 = bt2_y; // Y do topo
                float bt2_y2 = bt2_y + font_height; // Y da base
            
                // Verificação se o mouse está encima de algum botão (muda a cor)
                if ((mouse_x >= bt1_x1 && mouse_x <= bt1_x2) && (mouse_y >= bt1_y1 && mouse_y <= bt1_y2)) {
                    selected_option = 1;
                } else if ((mouse_x >= bt2_x1 && mouse_x <= bt2_x2) && (mouse_y >= bt2_y1 && mouse_y <= bt2_y2)) {
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
                    screen_done = true;
                } else if (selected_option == 2) {
                    screen_done = true;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                screen_done = true;
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
            
            // Reseta para a cor de fundo
            al_clear_to_color(fundo);

            // Desenha a imagem de fundo
            int background_width = al_get_bitmap_width(background);
            int background_heitgh = al_get_bitmap_height(background);
            al_draw_scaled_bitmap(background, 0, 0, background_width, background_heitgh, 0, 0, mundo->largura, mundo->altura, ALLEGRO_MAG_LINEAR);
            
            // Título
            al_draw_text(font_title, cor_normal, screen_x, title_y, ALLEGRO_ALIGN_CENTER, title);

            // Subtitulo (se tiver)
            if (subtitle) {
                al_draw_text(font_subtitle, cor_normal, screen_x, subtitle_y, ALLEGRO_ALIGN_CENTER, subtitle);
            }
            
            // Botão 1
            al_draw_text(font_button, (selected_option == 1) ? cor_selecionado : cor_normal, screen_x, bt1_y, ALLEGRO_ALIGN_CENTER, button1);
            // Botão 2
            al_draw_text(font_button, (selected_option == 2) ? cor_selecionado : cor_normal, screen_x, bt2_y, ALLEGRO_ALIGN_CENTER, button2);

            // Desenhar mouse falso
            int mouse_width = al_get_bitmap_width(mouse_cursor);
            int mouse_height = al_get_bitmap_height(mouse_cursor);
            al_draw_scaled_bitmap(mouse_cursor, 0, 0, mouse_width, mouse_height, mouse_x, mouse_y, 
                                    mouse_width * 0.02, mouse_height * 0.02, ALLEGRO_MIN_LINEAR);     
            
            al_flip_display();
        }
    }

    // Destroi variáveis para finalizar o programa
    al_destroy_bitmap(mouse_cursor);
    al_destroy_bitmap(background);
    al_destroy_font(font_button);
    al_destroy_font(font_title);
    al_destroy_font(font_subtitle);

    return selected_option;
}