#include <stdio.h>
#include <stdlib.h>

#include "erros.h"


void matar_falta_memoria() {
    fprintf(stderr, "Erro de alocação de memóra.\n");
    exit(1);
}

void matar_pont_nulo() {
    fprintf(stderr, "Ponteiro Nulo.\n");
    exit(1);
}

void matar_erro_incializar() {
    fprintf(stderr, "Erro ao inicializar allegro.\n");
    exit(1);
}