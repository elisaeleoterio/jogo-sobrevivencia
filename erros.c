#include <stdio.h>
#include <stdlib.h>

#include "erros.h"

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