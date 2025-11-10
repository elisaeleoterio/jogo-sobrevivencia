## Comando para executar o jogo
>``gcc menu.c -o menu $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_font-5)``
> Versão utilizada: 5.2

### TO-DO:
1. Implementar fundo
2. Como colocar sprites
3. Definir obstáculos
4. Menu
    a. Aumentar fonte
    b. Escalonar fundo
    c. Quadrado do menu
    d. Lógica dos botão
5. Função modular de verificação de colisão

### Allegro Addons:

allegro_image -> allegro
allegro_primitives -> allegro
allegro_color -> allegro

allegro_font -> allegro
allegro_ttf -> allegro_font -> allegro

allegro_audio -> allegro
allegro_acodec -> allegro_audio -> allegro
allegro_video -> allegro_audio -> allegro

allegro_memfile -> allegro
allegro_physfs -> allegro

allegro_native_dialog -> allegro