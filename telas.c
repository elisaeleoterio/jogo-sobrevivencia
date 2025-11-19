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

#define DISTANCIA_MAX 3000
#define TOLERANCIA 0.2

#define PLATAFORMA 1
#define ESPINHO 2
#define ANIMAL 3
#define BURACO_FIXO 4
#define BURACO_SURGE 5
#define PLATAFORMA_MOVEL 6
#define CAVERNA 7
#define PERSONAGEM 8


int menu(struct mundo *mundo) {
    if (!mundo) {
        matarProgramaErro(3);
    }
    
    // Carrega imagem de fundo do menu
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/menu_background.png");
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
    ALLEGRO_FONT *font = al_load_font("assets/fonts/Gafata-Regular.ttf", 36, 0);
    if (!font) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(mouse_cursor);
        matarProgramaErro(4);
    }

    // Variáveis de estado do menu
    bool menu_done = false;
    // Flag para controlar quando desenhar na tela
    bool redraw = true; 

    // Variáveis de estado do mouse e seleção
    int mouse_x = 0;
    int mouse_y = 0;
    // 0 = nenhum, 1 = Start, 2 = Exit
    int selected_option = 0; 

    // Loop principal do menu
    while (!menu_done) {
        
        // Armazena a altura a font
        int font_height = al_get_font_line_height(font);

        // Ponto X central
        float menu_x = mundo->largura / 2.0; 
        // Ponto Y central
        float menu_y = mundo->altura / 2.0;

        // // Tamanho do bloco do menu
        // float menu_width = 2 * mundo->largura / 3; // Largura total do bloco do menu
        // float menu_height = 2 * mundo->altura /3; // Altura total do bloco do menu

        // Espaçamento entre os itens
        float padding = font_height * 1; // Meia linha de espaço

        // Posições Y para desenhar o texto (al_draw_text usa o Y do topo)
        float title_y = menu_y - font_height - padding;
        float start_y = menu_y + padding;
        float exit_y = start_y + padding; 



        ALLEGRO_EVENT evento;
        // Espera por um evento (do usuário ou do timer)
        al_wait_for_event(mundo->fila_eventos, &evento);

        switch (evento.type) {
            // Realiza esse evento a cada tique do timer - Verificação da seleção das opções do menu
            case ALLEGRO_EVENT_TIMER: 
                // Obter as larguras dos textos
                float start_text_width = al_get_text_width(font, "START");
                float exit_text_width = al_get_text_width(font, "EXIT");

                // Definir as áreas das hitboxes
                // START
                float start_x1 = menu_x - (start_text_width / 2.0);
                float start_x2 = menu_x + (start_text_width / 2.0);
                float start_y1 = start_y; // Y do topo (usado no al_draw_text)
                float start_y2 = start_y + font_height; // Y da base

                // EXIT
                float exit_x1 = menu_x - (exit_text_width / 2.0);
                float exit_x2 = menu_x + (exit_text_width / 2.0);
                float exit_y1 = exit_y; // Y do topo
                float exit_y2 = exit_y + font_height; // Y da base
            
                // Verificação se o mouse está encima de algum botão
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
                // TESTE - NÃO ESPECIFICANDO QUAL BOTÃO FOI PRESSIONADO DO MOUSE
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
            ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            // ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            
            // Reseta para a cor de fundo
            al_clear_to_color(fundo);

            // Desenha a imagem de fundo
            int background_width = al_get_bitmap_width(background);
            int background_heitgh = al_get_bitmap_height(background);
            al_draw_scaled_bitmap(background, 0, 0, background_width, background_heitgh, 0, 0, mundo->largura, mundo->altura, ALLEGRO_MAG_LINEAR);
            
            // Desenha bloco do menu
            al_draw_filled_rectangle(mundo->largura/3, mundo->altura/3, 2 * mundo->largura / 3, 2* mundo->altura / 3, fundo);
            
            // Título
            al_draw_text(font, cor_normal, menu_x, title_y, ALLEGRO_ALIGN_CENTER, "MENU");
            // START
            al_draw_text(font, (selected_option == 1) ? cor_selecionado : cor_normal, menu_x, start_y, ALLEGRO_ALIGN_CENTER, "START");
            // EXIT
            al_draw_text(font, (selected_option == 2) ? cor_selecionado : cor_normal, menu_x, exit_y, ALLEGRO_ALIGN_CENTER, "EXIT");

            // Desenhar mouse falso
            int mouse_width = al_get_bitmap_width(mouse_cursor);
            int mouse_height = al_get_bitmap_height(mouse_cursor);
            al_draw_scaled_bitmap(mouse_cursor, 0, 0, mouse_width, mouse_height, mouse_x, mouse_y, 
                                    mouse_width * 0.08, mouse_height * 0.08, ALLEGRO_MIN_LINEAR);     
            
            // Copies or updates the front and back buffers so that what has been drawn 
            // previously on the currently selected display becomes visible on screen. 
            al_flip_display();
        }
    }

    // Destroi variáveis para finalizar o programa
    al_destroy_bitmap(mouse_cursor);
    al_destroy_bitmap(background);
    al_destroy_font(font);

    return selected_option;
}

int fase_zero(struct mundo *mundo) {
    if (!mundo) {
        matarProgramaErro(3);
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/backgrounf-fase0-placeholder.png");
    if (!background) {
        return 1;
    }

    bool game_done = false;
    bool redraw = true;

    struct hitbox *player = cria_hitbox(100, 100, 10, 15, 2, 0, -10, NULL, PERSONAGEM);
    float velocidade = 10;

    // Plataformas de subir
    struct obstacle *lista_plataforma = NULL;
    adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA));
    adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 300, mundo->chao_y - 100, 90, 100, -velocidade, 0, 0, NULL, PLATAFORMA));
    adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 200, mundo->chao_y - 150, 80, 150, -velocidade, 0, 0, NULL, PLATAFORMA));
    adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 100, mundo->chao_y - 200, 70, 200, -velocidade, 0, 0, NULL, PLATAFORMA));


    // // Obstáculo de espinhos
    // struct hitbox *espinho = cria_hitbox(mundo->largura + 200, mundo->chao_y - 100, 100, 50, -velocidade, 0, 0, NULL, ESPINHO);

    // // Obstáculo de animal que se move (VER COMO QUE FAZ PARA SE MOVER LATERALMENTE COM MAIOR CONSTÂNCIA)
    // struct hitbox *animal = cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA);

    // // Buraco no chão fixo
    // struct hitbox *buraco_fixo = cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA);

    // // Plataformas móveis
    // struct hitbox *plataforma1 = cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA);
    // struct hitbox *plataforma1 = cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA);

    // // Buraco longo -> Usar habilidade de voar
    // struct hitbox *plataforma1 = cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA);

    // Posição inicial do background
    float back_x = 0;
    // Chão do jogo
    float chao_y = mundo->altura - 200;

    while (!game_done) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(mundo->fila_eventos, &evento);
        
        switch (evento.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                ALLEGRO_KEYBOARD_STATE key;
                al_get_keyboard_state(&key);

                // Posições iniciais
                player->old_x = player->x;
                float old_back_x = back_x;

                
                // Movimentar no Eixo X:                
                movimenta_hitbox(player, key);
                movimenta_lista_obstaculos(lista_plataforma, key);
                movimenta_background(&back_x, velocidade, key);

                // Verifica colisão no eixo x -> Antes da atuação da gravidade
                verifica_colisao_obs_eixo_x(player, lista_plataforma, &back_x, old_back_x, key);
                
                player->chao = false;
                    
                // Implementação da simulação da gravidade
                player->speed_y += mundo->gravidade;
                player->y += player->speed_y;
                    
                verifica_colisao_obs_eixo_y(player, lista_plataforma);
    
                // Verifica chão
                if (player->y + player->height > chao_y ) {
                    player->y = chao_y - player->height;
                    player->speed_y = 0;
                    player->chao = true;
                }
                
                // Limites de movimentação da tela
                if (player->x < 0) player->x = 0;
                if (player->x + player->width > mundo->largura/2) player->x = mundo->largura/2 - 10;
                if (player->y < 0) player->y = 0;
                if (player->y > mundo->altura) player->y = mundo->altura - player->height; 
                
                if (player->steps >= DISTANCIA_MAX || player->life <= 0) {
                    game_done = true;
                }
                
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_done = true;
                break;
            default:
                break;
        }
        
        
        if (redraw && al_event_queue_is_empty(mundo->fila_eventos)) {
            
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            // Definir cores
            // ALLEGRO_COLOR fundo = al_map_rgb(14, 17, 22);
            // ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            // ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            // ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            ALLEGRO_COLOR ceu = al_map_rgb(135, 206, 235);
            
            int bg_w = al_get_bitmap_width(background);
            int bg_h = al_get_bitmap_height(background);
            
            al_clear_to_color(ceu);
            
            float offset_x = fmod(back_x, bg_w);
            
            // Desenha a imagem de fundo
            al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x, 0, bg_w, mundo->altura, 0);
            al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w, 0, bg_w, mundo->altura, 0);
            // TODO: Ver como colocar o fundo para o lado esquerdo
            
            // Meu player (temp)
            al_draw_filled_rectangle(player->x, player->y, player->x + player->width, player->y + player->height, al_map_rgb(255, 0, 0));
            
            // Começa em (0, chao_y) e vai até (largura da tela, altura da tela)
            al_draw_filled_rectangle(0, chao_y, mundo->largura, mundo->altura, al_map_rgb(0, 150, 0)); 

            // Desenha obstáculos de teste
            struct obstacle *atual = lista_plataforma;
            while (atual != NULL) {
                al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(0, 0, 255));
                atual = atual->next;              
            }

            
            al_flip_display();
            redraw = false;
        }
        
    }
    return 0; // TEMP
}