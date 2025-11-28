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
#include "screens.h"
#include "fase_zero.h"
#include "obstaculos.h"

#define DISTANCIA_MAX 2000
#define TOLERANCIA 0.2

int fase_zero(struct mundo *mundo) {
    if (!mundo) {
        matar_pont_nulo();
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/fase0_background.png");
    if (!background) {
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *vida = al_load_bitmap("assets/images/vida.png");
    if (!vida) {
        al_destroy_bitmap(background);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *nuvem = al_load_bitmap("assets/images/nuvem.png");
    if (!nuvem) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *tempestade = al_load_bitmap("assets/images/tempestade.png");
    if (!tempestade) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *espinho1 = al_load_bitmap("assets/images/espinho1.png");
    if (!espinho1) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *espinho2 = al_load_bitmap("assets/images/espinho2.png");
    if (!espinho2) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        al_destroy_bitmap(espinho1);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *prensa = al_load_bitmap("assets/images/prensa.png");
    if (!prensa) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        al_destroy_bitmap(espinho1);
        al_destroy_bitmap(espinho2);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *animal = al_load_bitmap("assets/images/animal.png");
    if (!animal) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        al_destroy_bitmap(espinho1);
        al_destroy_bitmap(espinho2);
        al_destroy_bitmap(prensa);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *fim = al_load_bitmap("assets/images/fim.png");
    if (!fim) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        al_destroy_bitmap(espinho1);
        al_destroy_bitmap(espinho2);
        al_destroy_bitmap(prensa);
        al_destroy_bitmap(animal);
        matar_falta_memoria();
    }

    ALLEGRO_FONT *font = al_load_font("assets/fonts/LaBelleAurore-Regular.ttf", 50, 0);
    if (!font) {
        al_destroy_bitmap(background);
        al_destroy_bitmap(vida);
        al_destroy_bitmap(nuvem);
        al_destroy_bitmap(tempestade);
        al_destroy_bitmap(espinho1);
        al_destroy_bitmap(espinho2);
        al_destroy_bitmap(prensa);
        al_destroy_bitmap(animal);
        al_destroy_bitmap(fim);
        matar_falta_memoria();
    }

    ALLEGRO_BITMAP *parado = al_load_bitmap("assets/images/parado.png");
    ALLEGRO_BITMAP *andando = al_load_bitmap("assets/images/andando.png");
    ALLEGRO_BITMAP *pulando = al_load_bitmap("assets/images/pulando.png");
    ALLEGRO_BITMAP *voando  = al_load_bitmap("assets/images/voando.png");
    ALLEGRO_BITMAP *abaixado = al_load_bitmap("assets/images/abaixado.png");

    bool game_done = false;
    float velocidade = 10; // velocidade de movimentação da tela (rolling background)
    float chao_y = mundo->altura - 200;

    struct hitbox *player = cria_hitbox(100, 100, 100, 100, 2, NULL, T_PERSONAGEM);
    configura_player(player, 2, -10);
    configura_sprites_player(player,  parado, 1, andando, 4, pulando, 3, voando, 4, abaixado, 1);
    
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
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(50, chao_y, 300, 50, -velocidade, nuvem, T_NUVEM));

        // nuvens simples (jogador pegar o jeito)
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(600, chao_y - 50, 150, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(950, chao_y - 100, 150, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(1300, chao_y - 50, 150, 50, -velocidade, nuvem, T_NUVEM));
        
        // plataforma com tempesade encima
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(1700, chao_y, 900, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_TEMPESTADE], cria_hitbox(1900, chao_y - player->height / 2 - player->height - 100, 500, player->height / 2 + 100, -velocidade, tempestade, T_TEMPESTADE));

        // plataforma com um animal
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(2700, chao_y - 250, 500, 50, -velocidade, nuvem, T_NUVEM));
        struct hitbox *inimigo = cria_hitbox(2900, chao_y - 300, 50, 50, -velocidade, animal, T_ANIMAL);
        configura_animal(inimigo, 300, 8); // Patrulha 300px
        adicionar_obstaculo(&obstaculos[L_ANIMAL], inimigo);

        // plataforma longa com prensa
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(3400, chao_y - 325, 1500, 50, -velocidade, nuvem, T_NUVEM));
        // jogador precisa esperar a prensa subir para passar
        struct hitbox *prensa1 = cria_hitbox(3600, chao_y - 850, 150, 300, -velocidade, prensa, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa1, 250, 7); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa1);
        struct hitbox *prensa2 = cria_hitbox(4000, chao_y - 850, 150, 300, -velocidade, prensa, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa2, 250, 6); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa2);
        struct hitbox *prensa3 = cria_hitbox(4400, chao_y - 850, 150, 300, -velocidade, prensa, T_NUVEM_MOVEL);
        configura_nuvem_movel(prensa3, 250, 7); // Sobe 250 pixels
        adicionar_obstaculo(&obstaculos[L_NUVEM_MOVEL], prensa3);
        
        // nuvens que desaparecem
        struct hitbox *buraco1 = cria_hitbox(5000, chao_y - 100, 150, 50, -velocidade, nuvem, T_BURACO);
        configura_buraco(buraco1, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco1);
        struct hitbox *buraco2 = cria_hitbox(5200, chao_y - 150, 150, 50, -velocidade, nuvem, T_BURACO);
        configura_buraco(buraco2, 180, false);
        struct hitbox *buraco3 = cria_hitbox(5400, chao_y - 100, 150, 50, -velocidade, nuvem, T_BURACO);
        configura_buraco(buraco3, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco3);
        struct hitbox *buraco4 = cria_hitbox(5600, chao_y - 125, 150, 50, -velocidade, nuvem, T_BURACO);
        configura_buraco(buraco4, 180, false);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco4);
        struct hitbox *buraco5 = cria_hitbox(5800, chao_y - 150, 150, 50, -velocidade, nuvem, T_BURACO);
        configura_buraco(buraco5, 180, true);
        adicionar_obstaculo(&obstaculos[L_BURACO], buraco5);

        // circuito nuvem e espinho
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(6100, chao_y - 100, 150, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(6400, chao_y - 50, 150, 50, -velocidade, espinho1, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(6700, chao_y - 100, 100, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_ESPINHO], cria_hitbox(7000, chao_y - 50, 150, 50, -velocidade, espinho1, T_NUVEM));

        // nuvens finais
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(7300, chao_y - 50, 150, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(7500, chao_y - 100, 150, 50, -velocidade, nuvem, T_NUVEM));
        adicionar_obstaculo(&obstaculos[L_NUVEM], cria_hitbox(7700, chao_y - 50, 150, 50, -velocidade, nuvem, T_NUVEM));

        // estrela troféu do jogo
        adicionar_obstaculo(&obstaculos[L_FIM], cria_hitbox(7800, chao_y - 200, 70, 70, -velocidade, fim, T_FIM));
    
        float back_x = 0; // variável para controlar a posição do fundo
        bool redraw = true;
    
        // Bool para reiniciar a fase caso o jogador perca vida
        bool tentativa_done = false;

        // Loop da fase para reiniciar fase (jogador perde uma vida)
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
    
                    // Movimentar no Eixo X               
                    movimenta_hitbox(player, key);
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        movimenta_lista_obstaculos(obstaculos[i], key);
                    }
                    movimenta_background(&back_x, velocidade, key);

                    // Atualização específicas para alguns obstáculos
                    atualiza_lista_animais(obstaculos[L_ANIMAL]);
                    atualiza_lista_nuvem_movel(obstaculos[L_NUVEM_MOVEL]);
                    atualiza_lista_buracos(obstaculos[L_BURACO]);
    
                    // Verificação de colisão de obstáculos com funções especiais
                    if (colide_obs(player, obstaculos[L_ESPINHO]) || colide_obs(player, obstaculos[L_NUVEM_MOVEL]) 
                    || colide_obs(player, obstaculos[L_ANIMAL]) || colide_obs(player, obstaculos[L_TEMPESTADE])) {
                        player->life --;
                        tentativa_done = true;
                    }

                    // Se encostar na estrela, finaliza o jogo
                    if (verifica_colisao(player, obstaculos[L_FIM]->hitbox)) {
                        return 0;
                    }

                    // Verifica colisão no eixo x para alterar a movimentação
                    bool mundo_parou = false;
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        if (verifica_colisao_obs_eixo_x(player, obstaculos[i], &back_x, old_back_x, key)) {
                            mundo_parou = true;
                        }
                    }

                    // verifica se o jogador está "preso" por algum obstáculo
                    if (mundo_parou) {
                        for (int i = 0; i < NUM_LISTAS; i++) {
                            reverte_pos_lista(obstaculos[i]);
                        }
                    }
                    
                    player->chao = false;

                    // Implementação da simulação da gravidade
                    player->speed_y += mundo->gravidade;
                    player->y += player->speed_y;


                    for (int i = 0; i < NUM_LISTAS; i++) {
                        verifica_colisao_obs_eixo_y(player, obstaculos[i]);
                    }

                    // Se cair muito (caiu de plataformas), morre
                    if (player->y > mundo->altura + 100) {
                        player->life--;
                        tentativa_done = true;
                    }
                    
                    for (int i = 0; i < NUM_LISTAS; i++) {
                        salva_pos_anterior_lista(obstaculos[i]);
                    }
    
                    // Limites de movimentação da tela (mentém o jagod na tela e no máx até o meio)
                    if (player->x < 0) player->x = 0;
                    if (player->x + player->width > mundo->largura/2) player->x = mundo->largura/2 - 10;
                    if (player->y < 0) player->y = 0;
                    
                    break;
                case ALLEGRO_EVENT_KEY_DOWN: // Apresenta tela pause quando aperta ESC
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                            // Botão 1 = Retornar | Botão 2 = Sair
                            int selection = screen(mundo, "Pause", NULL, "Retornar", "Sair");
                            if (selection == 1) {
                                continue;
                            } else {
                                return 2;
                                game_done = true;
                                tentativa_done = true; 
                            }
                        }
                        break;
                    case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        game_done = true;
                        break;
                default:
                    break;
            }
            
            
            if (redraw && al_event_queue_is_empty(mundo->fila_eventos)) {
                
                // Desenhar fundo
                al_clear_to_color(al_map_rgb(0, 0, 0));
    
                int bg_w = al_get_bitmap_width(background);
                int bg_h = al_get_bitmap_height(background);
                
                float offset_x = fmod(back_x, bg_w);

                if (bg_w < mundo->largura) {
                     al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w*2, 0, bg_w, mundo->altura, 0);
                }
                
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x, 0, bg_w, mundo->altura, 0);
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x + bg_w, 0, bg_w, mundo->altura, 0);
                al_draw_scaled_bitmap(background, 0, 0, bg_w, bg_h, -offset_x - bg_w, 0, bg_w, mundo->altura, 0);
                // TODO: Ver como colocar o fundo para o lado esquerdo

                // Desenhar personagem
                desenha_personagem(player, 0);
                
                // Desenhar os obstáculos
                for (int i = 0; i < NUM_LISTAS; i++) {
                    desenha_lista_obst(obstaculos[i]);
                }
                
                // Desenhar barra de voo
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
                
                // Pressionar esc para pausar
                al_draw_text(font, cor_jet, mundo->largura - al_get_text_width(font, "ESC para pausar"), 10, ALLEGRO_ALIGN_RIGHT, "ESC para pausar");
                
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
    al_destroy_bitmap(nuvem);
    al_destroy_bitmap(espinho1);
    al_destroy_bitmap(espinho2);
    al_destroy_font(font);
    return 1;
}