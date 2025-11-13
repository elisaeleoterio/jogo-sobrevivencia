// TENTAR FAZER TELA COM CHÃO E QUADRADO QUE SE MEXE DENTRO DO LIMITE DO DISPLAY

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

int main() {
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    al_install_keyboard();

    ALLEGRO_DISPLAY *display = al_create_display(640, 320);
    if (!display) {
        return 1;
    }

    // Criação da fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        al_destroy_display(display);
        return 1;
    }

    // Cria um timer (relógio) do jogo para rodar a 60 FPS (frames per second)
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        al_destroy_display(display);
        al_destroy_event_queue(fila_eventos);
        return 1;
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/backgrounf-fase0-placeholder.png");
    if (!background) {
        return 1;
    }

    // Registra as fontes dos eventos buscados no programa
    al_register_event_source(fila_eventos, al_get_display_event_source(display)); // Eventos da tela
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));     // Eventos do timer (FPS)
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());          // Eventos do teclado

    // Esconde o cursor do mouse
    al_hide_mouse_cursor(display);

    // Inicia o timer
    al_start_timer(timer);
    bool game_done = false;
    bool redraw = true;

    float red_x = 100;
    float red_size = 10;
    float red_y = 100;
    float vel_jogador = 5;
    
    // Posição inicial do background
    float back_x = 0;
    float back_y = 0;

    while (!game_done) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        
        switch (evento.type) {
            case ALLEGRO_EVENT_TIMER:
                ALLEGRO_KEYBOARD_STATE key;
                al_get_keyboard_state(&key);

                if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) {
                    // Move a tela para direita
                    back_x = back_x + vel_jogador;
                    // Altera posição do jogador
                    red_x+=1;   
                }
                if (al_key_down(&key, ALLEGRO_KEY_LEFT)) {
                    back_x = back_x - vel_jogador;
                    red_x-=1;  
                }
                if (al_key_down(&key, ALLEGRO_KEY_DOWN)) {
                    red_y+=1; 
                }
                if (al_key_down(&key, ALLEGRO_KEY_UP)) {
                    red_y-=1; 
                }
                
                
                redraw = true;
                if (red_x < 0) {
                    red_x = 0;
                }
    
                if (red_x + red_size > 640/2) {
                    red_x = 640/2 - 10;
                }
            
                if (red_y < 0) {
                    red_y = 0;
                }
    
                if (red_y + 2 > 320) {
                    red_y = 320 - red_size; 
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_done = true;
                break;
            default:
                break;
        }
        
        
        if (redraw && al_event_queue_is_empty(fila_eventos)) {
            
            
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_rectangle(0, 0, 640, 320, al_map_rgb(255, 0, 0), 2);

            // Jogador temporário
            
            // Definir cores
            ALLEGRO_COLOR fundo = al_map_rgb(14, 17, 22);
            ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            // ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            ALLEGRO_COLOR ceu = al_map_rgb(135, 206, 235);
            
            // Reseta para a cor de fundo
            al_clear_to_color(fundo);
            
            int bg_w = al_get_bitmap_width(background);
            int bg_h = al_get_bitmap_height(background);
            
            al_clear_to_color(ceu);
            
            float offset_x = fmod(back_x, bg_w);
            float draw_y = 320 - bg_h;
            
            // Desenha a imagem de fundo
            al_draw_scaled_bitmap(background, 0, 0,        // sx, sy (Pegar a imagem toda)
                bg_w, bg_h,   // sw, sh (A imagem toda)
                -offset_x, 0, // dx, dy (Posição na tela)
                bg_w, 320, // dw, dh (LARGURA original, ALTURA da tela)
                0);
                
            // 3. Desenha a segunda cópia (a que "dá a volta")
            // Ela é desenhada exatamente à direita da primeira,
            // preenchendo o espaço vazio que aparece durante o scroll.
            al_draw_scaled_bitmap(background,
                                    0, 0,        // sx, sy
                                    bg_w, bg_h,   // sw, sh
                                    -offset_x + bg_w, 0, // dx, dy (Deslocada pela largura)
                                    bg_w, 320, // dw, dh (Também esticada)
                                    0);
                    
                al_draw_filled_rectangle(red_x, red_y, red_x + 10, red_y + 10, al_map_rgb(255, 0, 0));
            
            al_flip_display();
            redraw = false;
        }
        
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);

    return 0;
}