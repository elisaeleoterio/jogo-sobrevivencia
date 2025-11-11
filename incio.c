#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "erros.h"
#include "inicio.h"

// FUNÇÕES DE INICIALIZAÇÃO DO JOGO E DAS TELAS (ADDONS E VARIÁVEIS)

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
