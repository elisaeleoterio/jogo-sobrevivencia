// TENTAR FAZER TELA COM CHÃO E QUADRADO QUE SE MEXE DENTRO DO LIMITE DO DISPLAY

#include <stdio.h>
#include <stdlib.h>
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

    // Registra as fontes dos eventos buscados no programa
    al_register_event_source(fila_eventos, al_get_display_event_source(display)); // Eventos da tela
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));     // Eventos do timer (FPS)
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());          // Eventos do teclado

    // Esconde o cursor do mouse
    al_hide_mouse_cursor(display);


    float x, y;
    x = 100;
    y = 100;

    // Inicia o timer
    al_start_timer(timer);
    bool game_done = false;
    bool redraw = true;


    while (!game_done) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        
        switch (evento.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_CHAR: 
                switch (evento.keyboard.keycode) {
                    case ALLEGRO_KEY_RIGHT:
                        x+=10;                    
                        break;
                    case ALLEGRO_KEY_LEFT:
                        x-=10;
                        break;
                    case ALLEGRO_KEY_UP:
                        y-=10;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        y+=10;
                    default:
                        break;
                }
            default:
                break;
        }


        if (redraw && al_event_queue_is_empty(fila_eventos)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(x, y, x + 10, y + 10, al_map_rgb(255, 0, 0));

            al_flip_display();
            redraw = false;
        }
        
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
