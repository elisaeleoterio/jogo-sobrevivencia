#include <stdio.h>
#include <allegro5/allegro.h>

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
    default:
        break;
    }
}

int main() {
    
    // Inicialização do Allegro no programa
    if(!al_init()) {
        matarProgramaErro(1); 
    }

    // Describes a monitor’s size and position relative to other monitors. 
    // x1, y1 will be 0, 0 on the primary display. 
    // Other monitors can have negative values if they are to the left or above the primary display. 
    // x2, y2 are the coordinates one beyond the bottom right pixel, so that x2-x1 gives 
    // the width and y2-y1 gives the height of the display.

    ALLEGRO_MONITOR_INFO monitor_info;
    // Pegar a informação do monitor em que o jogo será rodado
    // 0 indica o monitor principal
    if (!al_get_monitor_info(0, &monitor_info)) {
        matarProgramaErro(2);
    }

    // Definir a altura e largura do monitor
    int width = monitor_info.x2 - monitor_info.x1;
    int height = monitor_info.y2 - monitor_info.y1;

    // Setar a flag de FULLSCREEN_WINDOW - faz com que consuma a tela toda, tirando as bordas
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

    // Criar display do jogo
    ALLEGRO_DISPLAY *display = al_create_display(width, height);
    if (!display) {
        matarProgramaErro(2);
    }

    al_clear_to_color(al_map_rgb(25, 0, 25)); // Dark purple
    al_flip_display();
    al_rest(3.0); // Pause for 3 seconds
    

    al_destroy_display(display);
    return 0;
    
}


