#include <stdio.h>
#include <allegro5/allegro.h>

int main(void) {
    
    // Estas macros são substituídas por valores *antes* do código ser compilado
    
    printf("Compiled with Allegro version: %d.%d.%d (release %d)\n",
           ALLEGRO_VERSION,
           ALLEGRO_SUB_VERSION,
           ALLEGRO_WIP_VERSION,
           ALLEGRO_RELEASE_NUMBER);
           
    // Você também pode usar a macro de string
    printf("Compiled with version string: %s\n", ALLEGRO_VERSION_STR);

    return 0;
}