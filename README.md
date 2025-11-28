# TO-DO:
[ ] Implementar fundo
[ ] Como colocar sprites
[ ] Definir obstáculos
[ ] Menu
    [ ] Aumentar fonte
    [ ] Escalonar fundo
    [ ] Lógica dos botão
[ ] Função modular de verificação de colisão
[ ] Ver como que funciona gravidade pro boneco pular
[ ] Ajeitar sprites
[ ] Ver como carregar obstáculos
    - Tem que aparecer ao poucos, quando o jogador estiver no passo x
[ ] Criar movimentação abaixado
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
[x] Mudar sprites de tudo
    [x] Player
    [x] Fundo
    [x] Obstáculos
[x] Colocar estrela de fim
[ ] Textinho inicial de história
[x] Player voar
[x] Sistema de vida (deixar sprites visíveis)
[x] Organizar código
[x] Mudar fontes
[x] Pegar sprite Tempestade
[x] Sprite Animal
[x] Sprite Prensa

# FUNCIONALIDADES EXTRAS:
[x][7],10 pts, Implementar um sistema de Stamina/Fôlego (o jogador ""cansa""
ao correr ou ao se balançar e deve se recuperar)
[x][3],5 pts,Implementar um botão de pausa para a fase.
[x][4],10 pts, Implementar a função de ""rastejar"" (movimentar-se abaixado)
para passar por áreas baixas.
[x][2],15 pts, Implementar, pelo menos, um segundo tipo de obstáculo dinâmico
inédito, diferente dos requisitos mínimos (ex: plataformas que caem ou
desmoronam)
[ ][11],5 pts, Item coletável que aumenta a barra de vida.

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

## Fase
- Pelo menos uma fase completa;
- Pelo menos 6 elementos de perigo/armadilhas espalhados pela fase.

### Obstáculos Fase 0
[x] Espinhos
[x] Animal que se move dentro de limite horizontal
[x] Buraco no chão fixo
[x] Plataforma que se move verticalmente
[x] Plataforma que caí (fazer nuvens que despencam) // Seguir a lógica do buraco
[x] Buraco no chão que surge a cada peŕiodo de tempo (não conta como obstáculo diferente)
[x] Plataforma


## FIX
[x] Só está voando para a direita
[x] Colisão lateral não está muito boa
[x] Trava ao colidir com animal
[x] Está travando na tela 5000 e poucos
[x] Arrumar sprite de fundo
[x] Arrumar saída na tela pause
[x] Arrumar lateral da sprite do sapo
[ ] Revisar hitbox.c
