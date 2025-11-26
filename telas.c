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

#define DISTANCIA_MAX 2000
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
            ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            
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

    struct hitbox *player = cria_hitbox(100, 100, 10, 15, 2, 0, -10, NULL, PERSONAGEM);
    float velocidade = 10;
    

    while (!game_done && player->life > 0) {
        
        // Plataformas de subir
        struct obstacle *lista_plataforma = NULL;
        adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 400, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, PLATAFORMA));
        adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 200, mundo->chao_y - 100, 100, 100, -velocidade, 0, 0, NULL, PLATAFORMA));
        adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura - 0, mundo->chao_y - 150, 100, 150, -velocidade, 0, 0, NULL, PLATAFORMA));
        adicionar_obstaculo(&lista_plataforma, cria_hitbox(mundo->largura + 200, mundo->chao_y - 200, 100, 200, -velocidade, 0, 0, NULL, PLATAFORMA));

        // Plataforma que caí quando o player encosta
        struct obstacle *plataforma_cai = NULL;
        adicionar_obstaculo(&plataforma_cai, cria_hitbox(mundo->largura + 3000, mundo->chao_y - 50, 100, 20, -velocidade, 0, 0, NULL, PLATAFORMA));
    
        // // Obstáculo de espinhos
        struct obstacle *espinho = NULL;
        adicionar_obstaculo(&espinho, cria_hitbox(mundo->largura - 300, mundo->chao_y - 30, 100, 30, -velocidade, 0, 0, NULL, ESPINHO));
        adicionar_obstaculo(&espinho, cria_hitbox(mundo->largura - 100  , mundo->chao_y - 40, 100, 40, -velocidade, 0, 0, NULL, ESPINHO));
        adicionar_obstaculo(&espinho, cria_hitbox(mundo->largura + 100, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, ESPINHO));
        adicionar_obstaculo(&espinho, cria_hitbox(mundo->largura + 300, mundo->chao_y - 60, 100, 60, -velocidade, 0, 0, NULL, ESPINHO));
    
        // Obstáculo de animal que se move
        struct hitbox *animal = cria_hitbox(mundo->largura + 800, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, ANIMAL);
        configura_patrulha(animal, 200, 5);
        struct obstacle *animais = NULL;
        adicionar_obstaculo(&animais, animal);
    
        // Buraco no chão fixo
        struct obstacle *fixed_hole = NULL;
        adicionar_obstaculo(&fixed_hole, cria_hitbox(mundo->largura + 1500, mundo->chao_y - 0.01, 100, 600, -velocidade, 0, 0, NULL, BURACO_FIXO));

        struct hitbox *buraco_surge = cria_hitbox(mundo->largura + 2000, mundo->chao_y, 50, 600, -velocidade, 0, 0, NULL, BURACO_SURGE);
        configura_buraco_periodico(buraco_surge, 120); // Aparece a cada 2 segundos
        struct obstacle *appear_hole = NULL;
        adicionar_obstaculo(&appear_hole, buraco_surge);

        // Obstáculo de plataforma que se move verticalmente
        struct hitbox *plat_m1 = cria_hitbox(mundo->largura + 2500, 0, 100, 700, -velocidade, 0, 0, NULL, PLATAFORMA_MOVEL);
        configura_plat_movel(plat_m1, 200, 2);
        struct obstacle *plat_movel = NULL;
        adicionar_obstaculo(&plat_movel, plat_m1);

        struct hitbox *obs_despenca = cria_hitbox(mundo->largura + 4000, mundo->chao_y - 50, 100, 50, -velocidade, 0, 0, NULL, ANIMAL);
        obs_despenca_horizontal(obs_despenca, -1, 8);
        struct obstacle *despenca = NULL;
        adicionar_obstaculo(&despenca, obs_despenca);
    
        // Posição inicial do background
        float back_x = 0;
        // Chão do jogo
        float chao_y = mundo->altura - 200;

        // Resetar Jogador para posição inicial
        player->x = 100;
        player->y = 100;
        player->speed_y = 0;
        player->steps = 0;
        player->chao = false;

        bool redraw = true;
    
        // Bool para reiniciar a fase caso o jogador perca vida
        bool tentativa_done = false;

        // Loop da fase para reiniciar se precisar (jogador perde uma vida)
        while (!tentativa_done) {
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
                    movimenta_lista_obstaculos(espinho, key);
                    movimenta_lista_obstaculos(animais, key);
                    movimenta_lista_obstaculos(fixed_hole, key);
                    movimenta_lista_obstaculos(appear_hole, key);
                    movimenta_lista_obstaculos(plat_movel, key);
                    movimenta_lista_obstaculos(plataforma_cai, key);
                    movimenta_lista_obstaculos(despenca, key);
                    movimenta_background(&back_x, velocidade, key);

                    atualiza_lista_patrulha(animais);
                    atualiza_lista_plat_movel(plat_movel);
                    atualiza_buracos_periodicos(appear_hole);
                    atualiza_lista_obs_despenca(despenca);
    
                    // Verifica colisão no eixo x -> Antes da atuação da gravidade
                    bool parou_plataforma = verifica_colisao_obs_eixo_x(player, lista_plataforma, &back_x, old_back_x, key);
                    bool parou_plat_movel = verifica_colisao_obs_eixo_x(player, plat_movel, &back_x, old_back_x, key);
                    bool parou_espinho = verifica_colisao_obs_eixo_x(player, espinho, &back_x, old_back_x, key);
                    bool parou_animal = verifica_colisao_obs_eixo_x(player, animais, &back_x, old_back_x, key);
                    bool parou_buracos = verifica_colisao_obs_eixo_x(player, fixed_hole, &back_x, old_back_x, key);
                    bool parou_buracos_surge = verifica_colisao_obs_eixo_x(player, appear_hole, &back_x, old_back_x, key);
                    bool parou_plat_cai = verifica_colisao_obs_eixo_x(player, plataforma_cai, &back_x, old_back_x, key);
                    bool parou_despenca = verifica_colisao_obs_eixo_x(player, despenca, &back_x, old_back_x, key);
    
                    if (parou_plataforma) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(despenca);
                    }
    
                    if (parou_espinho) {
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(plataforma_cai);
                        reverte_pos_lista(despenca);
                    }

                    if (parou_animal) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(plataforma_cai);
                        reverte_pos_lista(despenca);
                    }

                    if (parou_buracos) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(plataforma_cai);
                        reverte_pos_lista(despenca);
                    }
                    
                    if (parou_buracos_surge) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(plataforma_cai);
                        reverte_pos_lista(despenca);
                    }

                    if (parou_plat_movel) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plataforma_cai);
                        reverte_pos_lista(despenca);
                    }

                    if (parou_plat_cai) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(despenca);
                    }

                    if (parou_despenca) {
                        reverte_pos_lista(espinho);
                        reverte_pos_lista(lista_plataforma);
                        reverte_pos_lista(animais);
                        reverte_pos_lista(fixed_hole);
                        reverte_pos_lista(appear_hole);
                        reverte_pos_lista(plat_movel);
                        reverte_pos_lista(plataforma_cai);
                    }
                    
                    
                    
                    
                    if (colide_obs(player, espinho)) {
                        player->life --;
                        tentativa_done = true;
                    }

                    if (colide_obs(player, despenca)) {
                        player->life --;
                        tentativa_done = true;
                    }
                    

                    if (colide_obs(player, plat_movel)) {
                        player->life --;
                        tentativa_done = true;
                    }

                    if (colide_obs(player, animais)) {
                        player->life --;
                        tentativa_done = true;
                    }
                    
                    player->chao = false;
                        
                    // Implementação da simulação da gravidade
                    player->speed_y += mundo->gravidade;
                    player->y += player->speed_y;

                    bool in_hole = verifica_colisao_obs_eixo_y(player, fixed_hole);
                    bool in_hole_surge = verifica_colisao_obs_eixo_y(player, appear_hole);
                    if (colide_obs(player, plataforma_cai)) {
                        in_hole = true;
                    }
                    verifica_colisao_obs_eixo_y(player, lista_plataforma);
                    verifica_colisao_obs_eixo_y(player, espinho);
                    verifica_colisao_obs_eixo_y(player, animais);
                    verifica_colisao_obs_eixo_y(player, plat_movel);
                    verifica_colisao_obs_eixo_y(player, plataforma_cai);
                    verifica_colisao_obs_eixo_y(player, despenca);

                    // Verifica chão - SÓ SE NÃO ESTIVER NO BURACO
                    // Se in_hole for true, ignoramos o chao_y
                    if (!in_hole_surge && !in_hole && (player->y + player->height > chao_y)) {
                        player->y = chao_y - player->height;
                        player->speed_y = 0;
                        player->chao = true;
                    }

                    // Se cair muito (caiu no buraco e passou do ecrã), morre
                    if (player->y > mundo->altura + 100) {
                        player->life--;
                        tentativa_done = true;
                    }
                    
                    salva_pos_anterior_lista(lista_plataforma);
                    salva_pos_anterior_lista(espinho);
                    salva_pos_anterior_lista(animais);
                    salva_pos_anterior_lista(fixed_hole);
                    salva_pos_anterior_lista(appear_hole);
                    salva_pos_anterior_lista(plat_movel);
                    salva_pos_anterior_lista(plataforma_cai);
                    salva_pos_anterior_lista(despenca);
    
                    // Limites de movimentação da tela
                    if (player->x < 0) player->x = 0;
                    if (player->x + player->width > mundo->largura/2) player->x = mundo->largura/2 - 10;
                    if (player->y < 0) player->y = 0;
                    
                    if (player->steps >= DISTANCIA_MAX) {
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
    
                atual = espinho;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(255, 0, 0));
                    atual = atual->next;              
                }
    
                atual = animais;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(0, 255, 0));
                    atual = atual->next;              
                }
                
                atual = fixed_hole;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(0, 0, 0));
                    atual = atual->next;              
                }

                atual = appear_hole;
                while (atual != NULL) {
                    if (atual->hitbox->active) {
                        al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(30, 0, 0));
                    }
                    atual = atual->next;              
                }


                atual = plat_movel;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(55, 6, 0));
                    atual = atual->next;              
                }

                atual = plataforma_cai;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(55, 6, 0));
                    atual = atual->next;              
                }

                atual = despenca;
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(55, 6, 0));
                    atual = atual->next;              
                }

                al_flip_display();
                redraw = false;
            }            
        }

        // Limpeza da Memória da Tentativa (IMPORTANTE)
        destruir_lista_obstaculos(lista_plataforma);
        destruir_lista_obstaculos(espinho);
        destruir_lista_obstaculos(animais);
    }

    // Limpeza final
    destruir_hitbox(player);
    al_destroy_bitmap(background);
    
    if (player->life <= 0) printf("Game Over!\n");
    
    return 0; // TEMP
}