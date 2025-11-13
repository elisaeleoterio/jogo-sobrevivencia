#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> // Add-on de Imagem
#include <allegro5/allegro_font.h>   // Add-on de Fonte
#include <allegro5/allegro_ttf.h>

#include "erros.h"
#include "inicio.h"

#ifndef TELAS_H_
#define TELAS_H_

int menu(struct mundo *mundo);
int fase_zero(struct mundo *mundo);
// int fase_um(struct mundo *mundo);
// int fase_dois(struct mundo *mundo);

#endif