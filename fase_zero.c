#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "erros.h"
#include "inicio.h"
#include "hitbox.h"
#include "fase_zero.h"
#include "obstaculos.h"

#define DISTANCIA_MAX 2000
#define TOLERANCIA 0.2

int fase_zero(struct mundo *mundo) {
    if (!mundo) {
        matarProgramaErro(3);
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/fase0_background.png");
    if (!background) {
        return 1;
    }

    ALLEGRO_BITMAP *vida = al_load_bitmap("assets/images/vida.png");
    if (!vida) {
        al_destroy_bitmap(background);
        return 1;
    }

    ALLEGRO_BITMAP *nuvem1 = al_load_bitmap("assets/images/nuvem1.png");
    if (!nuvem1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        return 1;
    }

   
    ALLEGRO_BITMAP *nuvem2 = al_load_bitmap("assets/images/nuvem2.png");
    if (!nuvem2) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem1);
        return 1;
    }

    ALLEGRO_BITMAP *espinho1 = al_load_bitmap("assets/images/espinho1.png");
    if (!espinho1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem1);
        al_destroy_bitmap(nuvem2);
        return 1;
    }

    ALLEGRO_BITMAP *espinho2 = al_load_bitmap("assets/images/espinho2.png");
    if (!espinho1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem1);
        al_destroy_bitmap(nuvem2);
        al_destroy_bitmap(espinho1);
        return 1;
    }

    ALLEGRO_FONT *font = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 50, 0);
    if (!font) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem1);
        al_destroy_bitmap(nuvem2);
        al_destroy_bitmap(espinho1);
        al_destroy_bitmap(espinho2);
        matarProgramaErro(4);
    }

    bool game_done = false;
    float velocidade = 10; // velocidade de movimentação da tela (rolling background)
    // Chão do jogo
    float chao_y = mundo->altura - 200;


    struct hitbox *player = cria_hitbox(100, 100, 10, 50, 2, NULL, T_PERSONAGEM);
    configura_player(player, 2, -10);
    
    // Vetor para armazenar as listas de obstáculos
    struct obstacle *obstaculos[NUM_LISTAS] = {NULL};

    // Loop para redesenhar caso morra
    while (!game_done && player->life > 0) {
        
        // Resetar Jogador para posição inicial
        player->x = 100;
        player->y = 100;
        player->speed_y = 0;
        player->steps = 0;
        player->chao = false;
        player->max_fly = 60; // 180 frames = 3 segundos de voo
        player->fly_timer = player->max_fly; // Começa cheio

        // nuvem pro jogador spwanar
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(50, chao_y, 300, 50, -velocidade, nuvem1, T_NUVEM));

        // nuvens simples (jogador pegar o jeito)
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(450, chao_y - 50, 150, 50, -velocidade, nuvem1, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(700, chao_y - 100, 150, 50, -velocidade, nuvem2, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(950, chao_y - 50, 150, 50, -velocidade, nuvem1, T_NUVEM));
        
        // Plataforma com espinho encima
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(1200, chao_y, 800, 50, -velocidade, nuvem2, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_TEMPESTADE], cria_hitbox(1400, chao_y - player->height / 2 - player->height, 500, player->height / 2 + 10, -velocidade, NULL, T_TEMPESTADE));

        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(2100, chao_y - 50, 100, 50, -velocidade, nuvem2, T_NUVEM));

        // plataforma com um animal
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(2300, chao_y - 250, 500, 50, -velocidade, nuvem2, T_NUVEM));
        struct hitbox *inimigo = cria_hitbox(2400, chao_y - 300, 50, 50, -velocidade, NULL, T_ANIMAL);
        configura_animal(inimigo, 300, 6); // Patrulha 300px
        adicionar_obstaculo(&obstaculos[L_ANIMAL], inimigo);

        // Plataforma longa com prensa
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3100, chao_y - 325, 1000, 50, -velocidade, nuvem2, T_NUVEM));
        // jogador precisa esperar a prensa subir para passar
        struct hitbox *prensa1 = cria_hitbox(3200, chao_y - 850, 150, 300, -velocidade, NULL, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa1, 250, 7); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa1);
        struct hitbox *prensa2 = cria_hitbox(3500, chao_y - 850, 150, 300, -velocidade, NULL, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa2, 250, 6); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa2);
        struct hitbox *prensa3 = cria_hitbox(3800, chao_y - 850, 150, 300, -velocidade, NULL, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa3, 250, 7); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa3);
        
        // Nuvens que desaparecem (fica translúcido)
        struct hitbox *buraco1 = cria_hitbox(4200, chao_y - 100, 150, 50, -velocidade, nuvem1, T_BURACO);
        configura_buraco(buraco1, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco1);
        struct hitbox *buraco2 = cria_hitbox(4250, chao_y - 150, 150, 50, -velocidade, nuvem1, T_BURACO);
        configura_buraco(buraco2, 180, false);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco2);
        struct hitbox *buraco3 = cria_hitbox(4500, chao_y - 100, 150, 50, -velocidade, nuvem2, T_BURACO);
        configura_buraco(buraco3, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco3);
        struct hitbox *buraco4 = cria_hitbox(4750, chao_y - 125, 150, 50, -velocidade, nuvem1, T_BURACO);
        configura_buraco(buraco4, 180, false);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco4);
        struct hitbox *buraco5 = cria_hitbox(4900, chao_y - 150, 150, 50, -velocidade, nuvem2, T_BURACO);
        configura_buraco(buraco5, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco5);

        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(5100, chao_y - 100, 150, 50, -velocidade, nuvem2, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(5200, chao_y - 20, 250, 20, -velocidade, espinho1, T_ESPINHO));
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(5500, chao_y, 150, 50, -velocidade, NULL, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(5700, chao_y - 50, 300, 50, -velocidade, nuvem1, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(5900, chao_y - 20, 250, 20, -velocidade, espinho2, T_ESPINHO));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(6000, chao_y - 100, 100, 50, -velocidade, nuvem2, T_NUVEM));
    
        float back_x = 0;

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
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        movimenta_lista_obstaculos(obstaculos[i], key);
                    }
                    movimenta_background(&back_x, velocidade, key);

                    // Atualização específicas para alguns obstáculos
                    atualiza_lista_animais(obstaculos[L_ANIMAL]);
                    atualiza_lista_nuvem_movel(obstaculos[L_NUVEM_MOVEL]);
                    atualiza_lista_buracos(obstaculos[L_BURACO]);
    
                    // Verifica colisão no eixo x -> Antes da atuação da gravidade
                    bool mundo_parou = false;
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        if (verifica_colisao_obs_eixo_x(player, obstaculos[i], &back_x, old_back_x, key)) {
                            mundo_parou = true;
                        }
                    }

                    if (mundo_parou) {
                        for (int i = 0; i < NUM_LISTAS; i++) {
                            reverte_pos_lista(obstaculos[i]);
                        }
                    }
                    
                    if (colide_obs(player, obstaculos[L_ESPINHO]) || colide_obs(player, obstaculos[L_NUVEM_MOVEL]) 
                    || colide_obs(player, obstaculos[L_ANIMAL]) || colide_obs(player, obstaculos[L_TEMPESTADE])) {
                        player->life --;
                        tentativa_done = true;
                    }
                    
                    // Gravidade e Colisão vertical
                    player->chao = false;
                        
                    // Implementação da simulação da gravidade
                    player->speed_y += mundo->gravidade;
                    player->y += player->speed_y;


                    for (int i = 0; i < NUM_LISTAS; i++) {
                        verifica_colisao_obs_eixo_y(player, obstaculos[i]);
                    }

                    // Se cair muito (caiu no buraco e passou do ecrã), morre
                    if (player->y > mundo->altura + 100) {
                        player->life--;
                        tentativa_done = true;
                    }
                    
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        salva_pos_anterior_lista(obstaculos[i]);
                    }
    
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
                
                int bg_w = al_get_bitmap_width(background);
                int bg_h = al_get_bitmap_height(background);
                
                float offset_x = fmod(back_x, bg_w);
                
                // Desenha a imagem de fundo
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x, 0, bg_w, mundo->altura, 0);
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w, 0, bg_w, mundo->altura, 0);
                // TODO: Ver como colocar o fundo para o lado esquerdo
                
                // Meu player (temp)
                al_draw_filled_rectangle(player->x, player->y, player->x + player->width, player->y + player->height, al_map_rgb(255, 0, 0));
    
                // Desenha todos os obstáculos na tela
                for (int i = 0; i < NUM_LISTAS; i++) {
                    desenha_lista_obst(obstaculos[i]);
                }
                
                // barra de voo
                al_draw_filled_rectangle(20, 40, 220, 60, al_map_rgb(50, 50, 50));
                // Cálculo da largura proporcional ao combustível
                float largura_combustivel = ((float)player->fly_timer / player->max_fly) * 200.0;
                ALLEGRO_COLOR cor_jet = al_map_rgb(221, 74, 147);
                al_draw_filled_rectangle(20, 40, 20 + largura_combustivel, 60, cor_jet);
                al_draw_text(font, cor_jet, 55 + largura_combustivel, 10, ALLEGRO_ALIGN_CENTER, "Voo");

                // Desenhar ícones de vida
                int vida_w = al_get_bitmap_width(vida);
                int vida_h = al_get_bitmap_height(vida);
                
                // Definições de tamanho na tela
                float tamanho_icone = 40.0; // Tamanho desejado (40x40)
                float espacamento = 10.0;
                float start_x = 20.0;
                float start_y = 80.0; // Logo abaixo da barra de voo

                for (int i = 0; i < player->life; i++) {
                    al_draw_tinted_scaled_bitmap(vida, cor_jet, 0, 0, vida_w, vida_h, start_x + (tamanho_icone + espacamento) * i, start_y, tamanho_icone, tamanho_icone, 0);
                }
                al_draw_text(font, cor_jet, start_x + (tamanho_icone + espacamento) * (player->life + 1), start_y - 20, ALLEGRO_ALIGN_CENTER, "Vida");
                
                al_flip_display();
                redraw = false;
            }            
        }

        // Limpeza dos obstáculos da tentaiva
        for (int i = 0; i < NUM_LISTAS; i++) {
            destruir_lista_obstaculos(obstaculos[i]);
            obstaculos[i] = NULL;
        }
    }

    // Limpeza final
    destruir_hitbox(player);
    al_destroy_bitmap(background);
    al_destroy_bitmap(vida);
    al_destroy_bitmap(nuvem1);
    al_destroy_bitmap(nuvem2);
    al_destroy_bitmap(espinho1);
    al_destroy_bitmap(espinho2);
    al_destroy_font(font);
    return 1;
}