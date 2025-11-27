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

    ALLEGRO_BITMAP *nuvem1 = al_load_bitmap("assets/images/nuvem1.png");
    if (!nuvem1) {
        al_destroy_bitmap(background);
        return 1;
    }
   
    ALLEGRO_BITMAP *nuvem2 = al_load_bitmap("assets/images/nuvem2.png");
    if (!nuvem2) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(nuvem1);
        return 1;
    }

    ALLEGRO_BITMAP *espinho1 = al_load_bitmap("assets/images/espinho1.png");
    if (!espinho1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(nuvem1);
        al_destroy_bitmap(nuvem2);
        return 1;
    }

    ALLEGRO_BITMAP *espinho2 = al_load_bitmap("assets/images/espinho2.png");
    if (!espinho1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(nuvem1);
        al_destroy_bitmap(nuvem2);
        al_destroy_bitmap(espinho1);
        return 1;
    }

    bool game_done = false;
    float velocidade = 10; // velocidade de movimentação da tela (rolling background)
    // Chão do jogo
    float chao_y = mundo->altura - 200;


    struct hitbox *player = cria_hitbox(100, 100, 10, 15, 2, NULL, T_PERSONAGEM);
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

        // // Nuvens de níveis diferentes (por onde anda normalmente)
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(50, chao_y - 50, 200, 50, -velocidade, NULL, T_NUVEM));
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(400, chao_y - 80, 100, 80, -velocidade, NULL, T_NUVEM));
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(mundo->largura - 200, mundo->chao_y - 100, 100, 100, -velocidade, NULL, T_NUVEM));
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(mundo->largura - 0, mundo->chao_y - 150, 100, 150, -velocidade, NULL, T_NUVEM));
        // adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(mundo->largura + 200, mundo->chao_y - 200, 100, 200, -velocidade, NULL, T_NUVEM));

        // // Nuvem com espinhos (sprite de raios)
        // adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(mundo->largura - 300, mundo->chao_y - 30, 100, 30, -velocidade, NULL, T_ESPINHO));
        // adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(mundo->largura - 100  , mundo->chao_y - 40, 100, 40, -velocidade, NULL, T_ESPINHO));
        // adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(mundo->largura + 100, mundo->chao_y - 50, 100, 50, -velocidade, NULL, T_ESPINHO));
        // adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(mundo->largura + 300, mundo->chao_y - 60, 100, 60, -velocidade, NULL, T_ESPINHO));
    
        // // Animal que se move
        // struct hitbox *animal = cria_hitbox(mundo->largura + 800, mundo->chao_y - 50, 100, 50, -velocidade, NULL, T_ANIMAL);
        // configura_animal(animal, 200, 5);
        // adicionar_obstaculo(&obstaculos[L_ANIMAL], animal);
    
        // // Buraco que surge periódicamente
        // struct hitbox *buraco_surge = cria_hitbox(mundo->largura + 2000, mundo->chao_y, 50, 600, -velocidade, NULL, T_BURACO);
        // configura_buraco(buraco_surge, 120); // Aparece a cada 2 segundos
        // adicionar_obstaculo(&obstaculos[L_BURACO], buraco_surge);

        // // Nuvem que se move verticalmente
        // struct hitbox *plat_m1 = cria_hitbox(mundo->largura + 2500, 0, 100, 700, -velocidade, NULL, T_NUVEM_MOVEL);
        // configura_nuvem_movel(plat_m1, 200, 2);
        // adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], plat_m1);

        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(50, chao_y, 300, 50, -velocidade, NULL, T_NUVEM));

        // Degraus Simples (Sobe e Desce)
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(450, chao_y - 50, 150, 50, -velocidade, NULL, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(700, chao_y - 100, 150, 50, -velocidade, NULL, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(950, chao_y - 50, 150, 50, -velocidade, NULL, T_NUVEM));
        
        // Plataforma de descanso
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(1200, chao_y, 200, 50, -velocidade, NULL, T_NUVEM));

        // --- PARTE 2: PLATAFORMA MÓVEL (ELEVADOR) ---
        // O jogador precisa esperar o elevador descer
        struct hitbox *elevador = cria_hitbox(1600, chao_y - 200, 150, 30, -velocidade, NULL, T_NUVEM_MOVEL);
        configura_nuvem_movel(elevador, 250, 3); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], elevador);

        // Plataforma Alta (Destino do elevador)
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(1850, chao_y - 100, 300, 50, -velocidade, NULL, T_NUVEM));

        // --- PARTE 3: O CORREDOR DO INIMIGO ---
        // Uma plataforma longa no alto com um inimigo patrulhando
        struct hitbox *plataforma_inimigo = cria_hitbox(2300, chao_y - 250, 500, 50, -velocidade, NULL, T_NUVEM);
        adicionar_obstaculo(&obstaculos[L_NUVEM], plataforma_inimigo);

        struct hitbox *inimigo = cria_hitbox(2400, chao_y - 300, 50, 50, -velocidade, NULL, T_ANIMAL);
        configura_animal(inimigo, 300, 4); // Patrulha 300px
        adicionar_obstaculo(&obstaculos[L_ANIMAL], inimigo);

        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3000, chao_y - 100, 150, 50, -velocidade, NULL, T_NUVEM));
        
        adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(3200, chao_y - 50, 50, 50, -velocidade, NULL, T_ESPINHO));
        
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3300, chao_y, 150, 50, -velocidade, NULL, T_NUVEM));

        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3600, chao_y - 50, 100, 50, -velocidade, NULL, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3800, chao_y - 100, 100, 50, -velocidade, NULL, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(4000, chao_y - 150, 100, 50, -velocidade, NULL, T_NUVEM));
        
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(4200, chao_y - 50, 600, 50, -velocidade, NULL, T_NUVEM));
    
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
                    for (size_t i = 0; i < NUM_LISTAS; i++) {
                        movimenta_lista_obstaculos(obstaculos[i], key);
                    }
                    movimenta_background(&back_x, velocidade, key);

                    // Atualização específicas para alguns obstáculos
                    atualiza_lista_animais(obstaculos[L_ANIMAL]);
                    atualiza_lista_nuvem_movel(obstaculos[L_NUVEM_MOVEL]);
                    atualiza_lista_buracos(obstaculos[L_BURACO]);
    
                    // Verifica colisão no eixo x -> Antes da atuação da gravidade
                    bool mundo_parou = false;
                    for (size_t i = 0; i < NUM_LISTAS; i++) {
                        if (verifica_colisao_obs_eixo_x(player, obstaculos[i], &back_x, old_back_x, key)) {
                            mundo_parou = true;
                        }
                    }

                    if (mundo_parou) {
                        for (size_t i = 0; i < NUM_LISTAS; i++) {
                            reverte_pos_lista(obstaculos[i]);
                        }
                    }
                    
                    if (colide_obs(player, obstaculos[L_ESPINHO]) || colide_obs(player, obstaculos[L_NUVEM_MOVEL]) || colide_obs(player, obstaculos[L_ANIMAL])) {
                        player->life --;
                        tentativa_done = true;
                    }
                    
                    // Gravidade e Colisão vertical
                    player->chao = false;
                        
                    // Implementação da simulação da gravidade
                    player->speed_y += mundo->gravidade;
                    player->y += player->speed_y;


                    bool in_hole = false;
                    for (size_t i = 0; i < NUM_LISTAS; i++) {
                        in_hole = verifica_colisao_obs_eixo_y(player, obstaculos[i]);
                    }

                    // Se cair muito (caiu no buraco e passou do ecrã), morre
                    if (player->y > mundo->altura + 100) {
                        player->life--;
                        tentativa_done = true;
                    }
                    
                    for (size_t i = 0; i < NUM_LISTAS; i++) {
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

                ALLEGRO_COLOR ceu = al_map_rgb(155, 106, 207); 
                
                int bg_w = al_get_bitmap_width(background);
                int bg_h = al_get_bitmap_height(background);
                
                float offset_x = fmod(back_x, bg_w);
                
                // Desenha a imagem de fundo
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x, 0, bg_w, mundo->altura, 0);
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w, 0, bg_w, mundo->altura, 0);
                // TODO: Ver como colocar o fundo para o lado esquerdo
                
                // Meu player (temp)
                al_draw_filled_rectangle(player->x, player->y, player->x + player->width, player->y + player->height, al_map_rgb(255, 0, 0));
    
                // Desenha obstáculos de teste
                struct obstacle *atual = obstaculos[L_NUVEM];
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(0, 0, 255));
                    atual = atual->next;              
                }
    
                atual = obstaculos[L_ESPINHO];
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(255, 0, 0));
                    atual = atual->next;              
                }
    
                atual = obstaculos[L_ANIMAL];
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(0, 255, 0));
                    atual = atual->next;              
                }

                atual = obstaculos[L_BURACO];
                while (atual != NULL) {
                    if (atual->hitbox->active) {
                        al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(30, 0, 0));
                    }
                    atual = atual->next;              
                }

                atual = obstaculos[L_NUVEM_MOVEL];
                while (atual != NULL) {
                    al_draw_filled_rectangle(atual->hitbox->x, atual->hitbox->y, atual->hitbox->x + atual->hitbox->width, atual->hitbox->y + atual->hitbox->height, al_map_rgb(55, 6, 0));
                    atual = atual->next;              
                }
                
                // --- DESENHO DA INTERFACE DO JETPACK ---
                // Barra de fundo (Cinza)
                al_draw_filled_rectangle(20, 20, 220, 40, al_map_rgb(50, 50, 50));
                
                // Cálculo da largura proporcional ao combustível
                float largura_combustivel = ((float)player->fly_timer / player->max_fly) * 200.0;
                
                // Barra de combustível (Amarela se cheio, Vermelha se acabando)
                ALLEGRO_COLOR cor_jet = al_map_rgb(255, 200, 0);
                if (player->fly_timer < 50) cor_jet = al_map_rgb(255, 50, 50);
                
                al_draw_filled_rectangle(20, 20, 20 + largura_combustivel, 40, cor_jet);

                al_flip_display();
                redraw = false;
            }            
        }

        // Limpeza dos obstáculos da tentaiva
        for (size_t i = 0; i < NUM_LISTAS; i++) {
            destruir_lista_obstaculos(obstaculos[i]);
            obstaculos[i] = NULL;
        }
    }

    // Limpeza final
    destruir_hitbox(player);
    al_destroy_bitmap(background);
    
    if (player->life <= 0) printf("Game Over!\n");
    
    return 0; // TEMP
}