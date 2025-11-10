#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

int main() {
    al_init(); // Inicia o jogo
    uint32_t version = al_get_allegro_version();
    int major = version >> 24;
    printf("Major: %ld\n", major);
    int minor = (version >> 16) & 255;
    printf("Minor: %ld\n", minor);
    int revision = (version >> 8) & 255;
    int release = version & 255;
    printf("Versão: %ld\n", version);


    const char *nome = "Nome do meu jogo";
    al_set_app_name(nome);

    const char *nomeApp = al_get_app_name();
    printf("Nome: %s\n", nome);

    al_install_keyboard();


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0); // Define o temporizador do programa
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue(); // Cria uma fila de eventos
    ALLEGRO_DISPLAY* display = al_create_display(1024, 1024); // Cria um display deste tamanho

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    int height = al_get_display_height(display);
    int width = al_get_display_width(display);
    printf("Height: %d\nWidth: %d\n", height, width);
    
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    while (timer) {
        al_wait_for_event(queue, &event);
        
        al_clear_to_color(al_map_rgb(1, 0.25, 1));
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            break;
        }
        
        if (al_is_event_queue_empty(queue)) {
            al_flip_display();
        }
    }

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
     al_destroy_display(display);
}
//     bool redraw = true;
//     ALLEGRO_EVENT event;

//     al_start_timer(timer);
//     while(1)
//     {
//         al_wait_for_event(queue, &event);

//         if(event.type == ALLEGRO_EVENT_TIMER)
//             redraw = true;
//         else if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
//             break;

//         if(redraw && al_is_event_queue_empty(queue))
//         {
//             al_clear_to_color(al_map_rgb(0, 0, 0));
//             al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
//             al_flip_display();

//             redraw = false;
//         }
//     }

//     al_destroy_font(font);
//     al_destroy_display(disp);
//     al_destroy_timer(timer);
//     al_destroy_event_queue(queue);

//     return 0;
// }