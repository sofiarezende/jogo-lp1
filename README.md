# Base Defense

Jogo 2D em C++17 + Allegro 5 sobre uma tartaruga defendendo seu ninho de ovos contra gaivotas que atacam pela praia.

## Recursos implementados

- [x] Heroi com vida limitada a 100.
- [x] Heroi com munição limitada e coleta de munição no mapa.
- [x] Movimento do heroi por WASD, com destino livre no mapa.
- [x] Disparo do heroi com `Q`, mirando no cursor no momento do tiro.
- [x] Inimigos surgindo nas bordas do mapa em intervalo que diminui com o tempo.
- [x] Inimigos com visual de gaivotas se movendo em direção ao heroi.
- [x] Inimigos atirando projeteis na direção do heroi.
- [x] Colisão de projeteis com heroi, inimigos e base.
- [x] Base com visual de ninho e ovos, vida limitada, regeneracao gradual e area centralizada ocupando 25% da tela.
- [x] Zona segura dentro da base: a tartaruga nao atira ali e recupera vida aos poucos.
- [x] Munição largada por inimigos derrotados, com tempo de expiracao.
- [x] Tela de vitoria ao sobreviver o tempo necessario.
- [x] Tela de derrota ao perder o heroi ou destruir a base.
- [x] Sprites carregados uma unica vez no `main.cpp` e recortados em sub-bitmaps.
- [x] Sons de ambiente, tiro e dano da base: `oceano.wav`, `tiros.wav` e `ovos_quebrando.ogg`.
- [x] Botao de replay na tela final.
- [x] Tartaruga usando a linha 4 verde do sprite sheet.
- [x] Projeteis do heroi usando o mesmo visual dos projeteis da gaivota.

## Conceitos usados

- `std::list<Inimigo>`: a lista de inimigos sofre remoções frequentes no meio da sequencia, então a lista encadeada simplifica erase durante o jogo.
- `std::list<Projetil>`: os projeteis tambem nascem e desaparecem o tempo todo, com iteracao e remoção no mesmo frame.
- `std::list<Municao>`: foi usada como estrutura equivalente a uma fila, porque o jogo precisa percorrer todos os itens do chao a cada quadro para detectar coleta e expiracao; uma `std::queue` pura nao permitiria essa verificacao completa.
- OOP separada em arquivos `.h` / `.cpp`: `Base`, `Heroi`, `Inimigo`, `Projetil`, `Municao` e `Jogo` ficam isolados por responsabilidade.
- Recorte de sprites com `al_create_sub_bitmap`: as folhas de sprite sao carregadas uma vez e reaproveitadas pelos objetos.
- Audio com Allegro: o jogo carrega samples uma vez, toca o oceano em loop e dispara os efeitos de tiro e dano no momento certo.

## Observacoes de gameplay

- A partida termina por vitoria ao sobreviver 10 minutos.
- A base fica no centro da tela e funciona como area segura.
- Os tiros do heroi usam a mesma linguagem visual dos projeteis das gaivotas.

## Como compilar

O projeto usa CMake.

No PowerShell, na raiz do projeto:

```powershell
cmake --build build
```

Se o ambiente ainda nao tiver as bibliotecas de audio do Allegro, o `CMakeLists.txt` ja foi ajustado para linkar `allegro_audio-5` e `allegro_acodec-5` junto dos demais modulos.

Se precisar reconfigurar do zero:

```powershell
cmake -S . -B build
cmake --build build
```

## Como executar

Execute o binario gerado em `build`:

```powershell
.\build\JogoLP1.exe
```

Se o jogo for iniciado a partir da pasta `build`, os assets sao localizados normalmente. O codigo tambem tenta a pasta `assets` da raiz como fallback.

Os arquivos de som ficam em `assets/som/`.

## Grupo
- Sofia Oliveira Rezende ()
- Thais Evelin Barbosa da Silva (20250039913)

