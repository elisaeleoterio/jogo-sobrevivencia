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
6. Ver boas práticas de nomeclatura de arquivos e funções
7. Ver como que funciona gravidade pro boneco pular
8. Ajeitar sprites
9. Ver como carregar obstáculos
    - Tem que aparecer ao poucos, quando o jogador estiver no passo x
10. Criar movimentação abaixado


### FUNCIONALIDADES EXTRAS:
20 pts, Implementar uma segunda fase, com background diferente e um
conjunto inédito de obstáculos.

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

### ERROS
- Não está verificando corretamente a colisão com um obstáculo




# REQUISITOS MÍNIMOS

- Implementação de um jogo singleplayer;
- Implementação de um menu inicial, permitindo iniciar ou sair do jogo;
- Implementação de uma tela de fim de jogo, indicando se o final foi alcançado com sucesso ou não (game over).

## Personagem:
- Sprites 2D (imagem) para as seguintes ações:
    - Em pé;
    - Abaixado;
    - Pulando;
    - Andando em pé;
    - Em ação de interação/desvio (ex: balançando ou escalando)
- Mobilidade geral: Andar, pular, e a ação de interação/desvio.
- Sistema de pontos de vida que reduz progressivamente conforme colisão recebida por perigos/armadilhas.

## Cenário
- Um cenário com background de imagem;
- Rolling background (o cenário muda conforme o jogador se
movimenta).

## Obstáculos
- Pelo menos 6 elementos de perigo e armadilhas espalhados pela fase, que o jogador deve desviar/saltar. Estes devem incluir pelo menos um tipo de perigo em movimento (ex: plataformas móveis, troncos rolando, animais que causam dano por contato).
- Todos os perigos/obstáculos devem ter suas respectivas sprites e movimento (se aplicável).
- Ataque (dano) é causado por contato/colisão, não por projéteis ou
combate.

### Obstáculos Fase 0
[x] Espinhos
[x] Animal que se move dentro de limite horizontal
[x] Buraco no chão fixo
[x] Plataforma que se move verticalmente
[x] Plataforma que caí (fazer nuvens que despencam) // Seguir a lógica do buraco
[ ] Obstáculo que "caí" pela tela vertical
[ ] Obstáculo que "caí" pela tela horizontal

[x] Buraco no chão que surge a cada peŕiodo de tempo (não conta como obstáculo diferente)
[x] Plataforma (não é obstáculo)
### Obstáculos Fase 1
[ ] Plataforma superior (player precisa se abaixar)
[ ] Prensas Hidráulicas
[ ] Plataformas que mudam de "estado" -> Nuvem transparente ou sólida


## Fase
- Pelo menos uma fase completa;
- Pelo menos 6 elementos de perigo/armadilhas espalhados pela fase.

# TODO Obrigatório
[ ] 3 Obstáculos
    [x] Tempestade (nuvem em cima)
    [x] Animal que se move
    [x] Prensa
    [x] Nuvem que aparece e desaparece (usar buraco)
    [x] Espinhos
    [ ] Raios verticais
    [x] Nuvem que tem que passar por baixo
[x] Movimentação de abaixar
[x] Tela de Game Over
[x] Tela de Fim (sucesso)
[ ] Mudar sprites de tudo
    [ ] Player
    [x] Fundo
    [ ] Obstáculos
[ ] Colocar estrela de fim
[ ] Textinho inicial de história
[x] Player voar
[x] Sistema de vida (deixar sprites visíveis)
[x] Organizar código
[x] Mudar fontes
[ ] Pegar sprite Tempestade
[ ] Sprite Animal
[ ] Sprite Prensa

## FIX
[ ] Só está voando para a direita
[ ] Colisão lateral não está muito boa
[ ] Trava ao colidir com animal
[ ] Está travando na tela 5000 e poucos
[ ] Arrumar sprite de fundo
