#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>

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

    ALLEGRO_BITMAP *background = al_load_bitmap("assets/images/background-placeholder.jpg");
    if (!background) {
        al_destroy_display(display);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        matarProgramaErro(4);
    }

    ALLEGRO_FONT *font = al_load_font("assets/fonts/Gafata-Regular.ttf", 36, 0);
    if (!font) {
        al_destroy_display(display);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        al_destroy_bitmap(background);
        matarProgramaErro(4);
    }

    // Variáveis de estado do menu
    bool game_done = false;
    // Flag para controlar quando desenhar na tela
    bool redraw = true; 

    // Variáveis de estado do mouse e seleção
    int mouse_x = 0;
    int mouse_y = 0;
    // 0 = nenhum, 1 = Start, 2 = Exit
    int selected_option = 0; 

    // Registra as fontes dos eventos buscados no programa
    al_register_event_source(fila_eventos, al_get_display_event_source(display)); // Eventos da tela
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));     // Eventos do timer (FPS)
    al_register_event_source(fila_eventos, al_get_mouse_event_source());          // Eventos do mouse

    // Esconde o cursor do mouse
    al_hide_mouse_cursor(display);

    // Inicia o timer
    al_start_timer(timer);

    // Loop principal do jogo
    while (!game_done) {
        
        // Armazena a altura a font
        int font_height = al_get_font_line_height(font);

        // int start_y_top, start_y_bottom, exit_y_top, exit_y_bottom, hitbox_half_width;

        // Ponto X central
        float menu_x = largura / 2.0; 

        // Ponto Y central (base do menu)
        float menu_y = altura / 2.0;

        // Espaçamento entre os itens
        float padding = font_height * 0.5; // Meia linha de espaço

        // Posições Y para desenhar o texto (al_draw_text usa o Y do topo)
        float title_y = menu_y - font_height - padding;
        float start_y = menu_y + padding;
        float exit_y = start_y + font_height; // Uma linha de fonte abaixo de "START"



        ALLEGRO_EVENT evento;
        // Espera por um evento (do usuário ou do timer)
        al_wait_for_event(fila_eventos, &evento);

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
                if (mouse_x >= start_x1 && mouse_x <= start_x2 && mouse_y >= start_y1 && mouse_y <= start_y2) {
                    selected_option = 1; 
                } 
                if (mouse_x >= exit_x1 && mouse_x <= exit_x2 && mouse_y >= exit_y1 && mouse_y <= exit_y2) {
                        selected_option = 2;
                }
                else {
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
                    game_done = true;
                } else if (selected_option == 2) {
                    printf("Botão EXIT selecionado.\n");
                    game_done = true;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game_done = true;
                break;
            default:
                break;
        }
        if (redraw && al_is_event_queue_empty(fila_eventos)) {
            redraw = false;

            
            // Definir cores
            ALLEGRO_COLOR fundo = al_map_rgb(14, 17, 22);
            ALLEGRO_COLOR cor_normal = al_map_rgb(200, 200, 200); // Cinza claro
            ALLEGRO_COLOR cor_selecionado = al_map_rgb(251, 116, 168); // Happy Pink
            ALLEGRO_COLOR mouse = al_map_rgb(255, 0, 127); // Bright Pink
            
            al_clear_to_color(fundo);
            al_draw_bitmap(background, 0, 0, 0);

            // Título
            al_draw_text(font, cor_normal, menu_x, title_y, ALLEGRO_ALIGN_CENTER, "MENU");
            // START
            al_draw_text(font, (selected_option == 1) ? cor_selecionado : cor_normal, menu_x, start_y, ALLEGRO_ALIGN_CENTER, "START");
            // EXIT
            al_draw_text(font, (selected_option == 2) ? cor_selecionado : cor_normal, menu_x, exit_y, ALLEGRO_ALIGN_CENTER, "EXIT");

            // Desenhar mouse falso
            al_draw_text(font, mouse, mouse_x, mouse_y, 0, "X");

            // Copies or updates the front and back buffers so that what has been drawn 
            // previously on the currently selected display becomes visible on screen. 
            al_flip_display();
        }
    }

    // Destroi variáveis para finalizar o programa
    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(display);

    printf("Fim do jogo.\n");
    return 0;
}