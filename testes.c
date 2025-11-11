#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>

#include "erros.h"
#include "inicio.h"

typedef struct {
    ALLEGRO_DISPLAY *display;
    int altura, largura;
    ALLEGRO_EVENT_QUEUE *fila_eventos;
    ALLEGRO_TIMER *timer;
} geral;

// FUNÇÃO DE ERRO
void matarProgramaErro(int codigo) {
    switch (codigo) {
    case 1:
        fprintf(stderr, "Erro de inicialização.\n");
        exit(1);
        break;
    case 2:
        fprintf(stderr, "Erro de chamada de função do allegro.\n");
        exit(1);
    case 3:
        fprintf(stderr, "Ponteiro Nulo.\n");
        exit(1);
    case 4:
        fprintf(stderr, "Erro de criação de variável.\n");
        exit(1);
    default:
        break;
    }
}


void inicializar() {
    if (!al_init()) {
        matarProgramaErro(1);
    }

    if (!al_init_image_addon()) {
        matarProgramaErro(1);
    }

    if (!al_init_font_addon()) {
        matarProgramaErro(1);
    }

    if (!al_init_ttf_addon())
    {
        matarProgramaErro(1);
    }

    if (!al_install_mouse()) {
        matarProgramaErro(1);
    }

    if (!al_install_keyboard()) {
        matarProgramaErro(1);
    }
    
}

ALLEGRO_DISPLAY *criar_display(int *largura, int *altura) {
    if (!largura || !altura) {
        matarProgramaErro(3);
    }
    
    // Describes a monitor’s size and position relative to other monitors. 
    // x1, y1 will be 0, 0 on the primary display. 
    // Other monitors can have negative values if they are to the left or above the primary display. 
    // x2, y2 are the coordinates one beyond the bottom right pixel, so that x2-x1 gives 
    // the width and y2-y1 gives the height of the display.

    // Pegar a informação do monitor em que o jogo será rodado
    // 0 indica o monitor principal
    ALLEGRO_MONITOR_INFO monitor_info;
    if (!al_get_monitor_info(0, &monitor_info)) {
        matarProgramaErro(2);
    }

    // Definir a altura e largura do monitor
    *largura = monitor_info.x2 - monitor_info.x1;
    *altura = monitor_info.y2 - monitor_info.y1;

    // Setar a flag de FULLSCREEN_WINDOW - faz com que consuma a tela toda, tirando as bordas
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

    ALLEGRO_DISPLAY *display = al_create_display(*largura, *altura);
    if (!display) {
        matarProgramaErro(4);
    }

    return display;

}

int main() {


    // FUNÇÃO MENU:

    // Inicialização e criação de variáveis do programa
    inicializar();

    // Criação do display do jogo e definição da largura e altura da tela
    int largura, altura;
    ALLEGRO_DISPLAY *display = criar_display(&largura, &altura);
    if (!display) {
        matarProgramaErro(4);
    }
    
    // Criação da fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        al_destroy_display(display);
        matarProgramaErro(4);
    }

    // Cria um timer (relógio) do jogo para rodar a 60 FPS (frames per second)
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        al_destroy_display(display);
        al_destroy_event_queue(fila_eventos);
        matarProgramaErro(4);
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

    // Flag para controlar quando desenhar na tela
    bool redraw = true; 


    // FUNÇÃO FASE ZERO
    // Carregar fundo


    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/backgrounf-fase0-placeholder.png");
    if (!background) {
        matarProgramaErro(4);
    }
    
    // Posição inicial do sprite
    float jogador_x = 100;
    float jogador_y = 100;
    float vel_jogador = 5;

    // Posição inicial do background
    float back_x = 0;
    float back_y = 0;

    // Tamanho de cada frame
    const float FRAME_LARGURA = 32;
    const float FRAME_ALTURA = 32;

    bool rodando = true;
    while (rodando) {
        ALLEGRO_EVENT evento;
        // Espera por um evento (do usuário ou do timer)
        al_wait_for_event(fila_eventos, &evento);

        switch (evento.type) {
            case ALLEGRO_EVENT_TIMER:
                // al_draw_bitmap(background, 0, 0, 0);
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                printf("Input do teclado.\n");
                switch (evento.keyboard.keycode) {
                    case ALLEGRO_KEY_RIGHT: // Nunca vou alterar a posição do jogador, só da tela??
                        // Move a tela para direita
                        back_x = back_x + vel_jogador;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        back_x = back_x - vel_jogador;
                        // Move para a esquerda (apenas se não está no início)
                        break;
                    case ALLEGRO_KEY_DOWN:
                        // back_y = back_y - vel_jogador;
                        break;
                    case ALLEGRO_KEY_UP:
                        // Coloca a tela um pouco para cima
                        // back_y = back_y + vel_jogador;
                        break;
                    default:
                        break;
                }
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                rodando = false;
                break;
            default:
                break;
        }

        if (redraw && al_event_queue_is_empty(fila_eventos)) {
            redraw = false;

            // Definir cores
            ALLEGRO_COLOR fundo = al_map_rgb(14, 17, 22);
            ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            ALLEGRO_COLOR ceu = al_map_rgb(135, 206, 235);
            
            // Reseta para a cor de fundo
            al_clear_to_color(fundo);

            int bg_w = al_get_bitmap_width(background);
            int bg_h = al_get_bitmap_height(background);

            al_clear_to_color(ceu);

            float offset_x = fmod(back_x, bg_w);
            float draw_y = altura - bg_h;
            
            // Desenha a imagem de fundo
            al_draw_scaled_bitmap(background, 0, 0,        // sx, sy (Pegar a imagem toda)
                                  bg_w, bg_h,   // sw, sh (A imagem toda)
                                  -offset_x, 0, // dx, dy (Posição na tela)
                                  bg_w, altura, // dw, dh (LARGURA original, ALTURA da tela)
                                  0);

            // 3. Desenha a segunda cópia (a que "dá a volta")
            // Ela é desenhada exatamente à direita da primeira,
            // preenchendo o espaço vazio que aparece durante o scroll.
            al_draw_scaled_bitmap(background,
                                  0, 0,        // sx, sy
                                  bg_w, bg_h,   // sw, sh
                                  -offset_x + bg_w, 0, // dx, dy (Deslocada pela largura)
                                  bg_w, altura, // dw, dh (Também esticada)
                                  0);

            al_flip_display();
        }
        
    }
    
    
    return 0;
}
