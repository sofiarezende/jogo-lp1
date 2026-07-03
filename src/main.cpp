#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <algorithm>
#include <cstdio>
#include <vector>

#include "vetor2d.h"
#include "jogo.h"

namespace {
    constexpr int LARGURA = 1280;
    constexpr int ALTURA = 720;
    constexpr float ESCALA_TELA = 0.90f;
    constexpr float FPS = 60.0f;
    constexpr int FRAME_INIMIGO_LARGURA = 48;
    constexpr int FRAME_INIMIGO_ALTURA = 48;
    constexpr int FRAME_ITEM_LARGURA = 16;
    constexpr int FRAME_ITEM_ALTURA = 16;
    constexpr int FRAME_GAIVOTA_LARGURA = 48;
    constexpr int FRAME_GAIVOTA_ALTURA = 40;
    constexpr float REPLAY_X = 540.0f;
    constexpr float REPLAY_Y = 400.0f;
    constexpr float REPLAY_LARGURA = 200.0f;
    constexpr float REPLAY_ALTURA = 52.0f;

    ALLEGRO_BITMAP* carregarBitmapComFallback(const char* caminhoPrincipal, const char* caminhoAlternativo) {
        ALLEGRO_BITMAP* bitmap = al_load_bitmap(caminhoPrincipal);
        if (!bitmap && caminhoAlternativo) {
            bitmap = al_load_bitmap(caminhoAlternativo);
        }

        return bitmap;
    }

    ALLEGRO_SAMPLE* carregarSampleComFallback(const char* caminhoPrincipal, const char* caminhoAlternativo) {
        ALLEGRO_SAMPLE* sample = al_load_sample(caminhoPrincipal);
        if (!sample && caminhoAlternativo) {
            sample = al_load_sample(caminhoAlternativo);
        }

        return sample;
    }

    std::vector<ALLEGRO_BITMAP*> recortarSprites(ALLEGRO_BITMAP* sheet,
                                                 const std::vector<std::pair<int, int>>& posicoes,
                                                 int largura, int altura) {
        std::vector<ALLEGRO_BITMAP*> sprites;
        if (!sheet) {
            return sprites;
        }

        sprites.reserve(posicoes.size());
        for (const auto& posicao : posicoes) {
            sprites.push_back(al_create_sub_bitmap(sheet, posicao.first * largura, posicao.second * altura, largura, altura));
        }

        return sprites;
    }

    void destruirSprites(std::vector<ALLEGRO_BITMAP*>& sprites) {
        for (ALLEGRO_BITMAP* sprite : sprites) {
            if (sprite) {
                al_destroy_bitmap(sprite);
            }
        }
        sprites.clear();
    }

    Vetor2D direcaoDasTeclas(bool esquerda, bool direita, bool cima, bool baixo) {
        Vetor2D direcao;
        if (esquerda) direcao.x -= 1.0f;
        if (direita) direcao.x += 1.0f;
        if (cima) direcao.y -= 1.0f;
        if (baixo) direcao.y += 1.0f;
        return direcao;
    }

    void calcularTamanhoJanela(int& largura, int& altura) {
        ALLEGRO_MONITOR_INFO monitorInfo;
        if (!al_get_monitor_info(0, &monitorInfo)) {
            largura = LARGURA;
            altura = ALTURA;
            return;
        }

        const int larguraDisponivel = static_cast<int>((monitorInfo.x2 - monitorInfo.x1) * ESCALA_TELA);
        const int alturaDisponivel = static_cast<int>((monitorInfo.y2 - monitorInfo.y1) * ESCALA_TELA);

        const float escalaLargura = static_cast<float>(larguraDisponivel) / static_cast<float>(LARGURA);
        const float escalaAltura = static_cast<float>(alturaDisponivel) / static_cast<float>(ALTURA);
        const float escala = std::min(escalaLargura, escalaAltura);

        largura = std::max(960, static_cast<int>(LARGURA * escala));
        altura = std::max(540, static_cast<int>(ALTURA * escala));
    }

    Vetor2D converterParaCoordenadasLogicas(const ALLEGRO_DISPLAY* display, int x, int y) {
        const float larguraJanela = static_cast<float>(al_get_display_width(const_cast<ALLEGRO_DISPLAY*>(display)));
        const float alturaJanela = static_cast<float>(al_get_display_height(const_cast<ALLEGRO_DISPLAY*>(display)));
        return Vetor2D(
            static_cast<float>(x) * (static_cast<float>(LARGURA) / larguraJanela),
            static_cast<float>(y) * (static_cast<float>(ALTURA) / alturaJanela));
    }
}

void desenharHud(const Jogo& jogo, ALLEGRO_FONT* font) {
    const ALLEGRO_COLOR fundo = al_map_rgba(28, 26, 22, 170);
    al_draw_filled_rounded_rectangle(8, 8, 320, 102, 10, 10, fundo);

    al_draw_textf(font, al_map_rgb(255, 250, 230), 18, 18, 0,
                  "Tartaruga: %d/%d",
                  jogo.getVidaHeroi(), jogo.getVidaHeroiMaxima());
    al_draw_textf(font, al_map_rgb(255, 250, 230), 18, 38, 0,
                  "Ovos do ninho: %d/%d",
                  jogo.getVidaBase(), jogo.getVidaBaseMaxima());
    al_draw_textf(font, al_map_rgb(255, 250, 230), 18, 58, 0,
                  "Municao: %d | Sobrevivencia: %.0f/%.0f",
                  jogo.getMunicaoHeroi(), jogo.getTempoSobrevivido(), jogo.getTempoParaVencer());
    al_draw_text(font, al_map_rgb(255, 230, 170), 18, 78, 0, "Clique direito para mover | Q para atirar");
}

void desenharMensagemFinal(const Jogo& jogo, ALLEGRO_FONT* font) {
    const char* mensagem = (jogo.getEstado() == EstadoJogo::VITORIA)
        ? "VITORIA!"
        : "DERROTA!";

    const char* subtitulo = (jogo.getEstado() == EstadoJogo::VITORIA)
        ? "A base sobreviveu ate o fim do tempo"
        : "A base foi destruida";

    int larguraTitulo = al_get_text_width(font, mensagem);
    int larguraSubtitulo = al_get_text_width(font, subtitulo);

    al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA / 2 - larguraTitulo / 2, ALTURA / 2 - 32, 0, mensagem);
    al_draw_text(font, al_map_rgb(255, 255, 255), LARGURA / 2 - larguraSubtitulo / 2, ALTURA / 2 + 2, 0, subtitulo);

    al_draw_filled_rounded_rectangle(REPLAY_X, REPLAY_Y, REPLAY_X + REPLAY_LARGURA, REPLAY_Y + REPLAY_ALTURA, 12, 12, al_map_rgb(72, 109, 143));
    al_draw_rounded_rectangle(REPLAY_X, REPLAY_Y, REPLAY_X + REPLAY_LARGURA, REPLAY_Y + REPLAY_ALTURA, 12, 12, al_map_rgb(255, 255, 255), 2.0f);

    const char* textoReplay = "REPLAY";
    int larguraReplay = al_get_text_width(font, textoReplay);
    al_draw_text(font, al_map_rgb(255, 255, 255), REPLAY_X + REPLAY_LARGURA / 2.0f - larguraReplay / 2.0f, REPLAY_Y + 16, 0, textoReplay);
}


int main(int argc, char **argv) {
    if(!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_install_audio();
    al_init_acodec_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_reserve_samples(16);

    int larguraJanela = LARGURA;
    int alturaJanela = ALTURA;
    calcularTamanhoJanela(larguraJanela, alturaJanela);

    ALLEGRO_DISPLAY* display = al_create_display(larguraJanela, alturaJanela);
    if (!display) {
        fprintf(stderr, "Falha ao criar a janela do jogo.\n");
        return -1;
    }

    ALLEGRO_MONITOR_INFO monitorInfo;
    if (al_get_monitor_info(0, &monitorInfo)) {
        al_set_window_position(
            display,
            monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - larguraJanela) / 2,
            monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - alturaJanela) / 2);
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    bool teclaEsquerda = false;
    bool teclaDireita = false;
    bool teclaCima = false;
    bool teclaBaixo = false;

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    ALLEGRO_BITMAP* heroSheet = carregarBitmapComFallback("../assets/tartaruga.png", "assets/tartaruga.png");
    ALLEGRO_BITMAP* backgroundSheet = carregarBitmapComFallback("../assets/background.png", "assets/background.png");
    ALLEGRO_BITMAP* gaivotaSheet = carregarBitmapComFallback("../assets/gaivota.png", "assets/gaivota.png");
    ALLEGRO_BITMAP* itemSheet = carregarBitmapComFallback("../assets/shoreThings_PaperPluto_demo.png", "assets/shoreThings_PaperPluto_demo.png");

    ALLEGRO_SAMPLE* somFundo = carregarSampleComFallback("../assets/som/oceano.wav", "assets/som/oceano.wav");
    ALLEGRO_SAMPLE* somTiro = carregarSampleComFallback("../assets/som/tiros.wav", "assets/som/tiros.wav");
    ALLEGRO_SAMPLE* somDanoArea = carregarSampleComFallback("../assets/som/ovos_quebrando.ogg", "assets/som/ovos_quebrando.ogg");

    if (somFundo) {
        al_play_sample(somFundo, 0.35f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, nullptr);
    }

    ALLEGRO_BITMAP* spriteGaivota = nullptr;
    if (gaivotaSheet) {
        spriteGaivota = al_create_sub_bitmap(gaivotaSheet, 9 * FRAME_GAIVOTA_LARGURA, 5 * FRAME_GAIVOTA_ALTURA, FRAME_GAIVOTA_LARGURA, FRAME_GAIVOTA_ALTURA);
    }

    std::vector<ALLEGRO_BITMAP*> spritesHeroi = recortarSprites(
        heroSheet,
        {{0, 3}, {1, 3}, {2, 3}},
        FRAME_INIMIGO_LARGURA,
        FRAME_INIMIGO_ALTURA);

    std::vector<ALLEGRO_BITMAP*> spritesItens = recortarSprites(
        itemSheet,
        {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
        FRAME_ITEM_LARGURA,
        FRAME_ITEM_ALTURA);

    al_start_timer(timer);

    Jogo jogo(LARGURA, ALTURA, somTiro, somDanoArea);

    bool rodando = true;
    bool redesenhando = true;

    while(rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            float deltaTime = 1.0f / FPS;

            Vetor2D direcaoTeclado = direcaoDasTeclas(teclaEsquerda, teclaDireita, teclaCima, teclaBaixo);
            if (jogo.getEstado() == EstadoJogo::JOGANDO && direcaoTeclado.magnitude() > 0.0f) {
                jogo.processarMovimentoTeclado(direcaoTeclado, deltaTime);
            }
            
            jogo.atualizar(deltaTime);

            redesenhando = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            Vetor2D posicaoMouse = converterParaCoordenadasLogicas(display, event.mouse.x, event.mouse.y);

            if (jogo.getEstado() != EstadoJogo::JOGANDO && event.mouse.button == 1) {
                bool dentroReplay = posicaoMouse.x >= REPLAY_X && posicaoMouse.x <= REPLAY_X + REPLAY_LARGURA &&
                                    posicaoMouse.y >= REPLAY_Y && posicaoMouse.y <= REPLAY_Y + REPLAY_ALTURA;
                if (dentroReplay) {
                    jogo.reiniciar();
                    teclaEsquerda = false;
                    teclaDireita = false;
                    teclaCima = false;
                    teclaBaixo = false;
                }
            } else if (event.mouse.button == 2 && jogo.getEstado() == EstadoJogo::JOGANDO) {
                jogo.processarClique(posicaoMouse);
            }
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A) {
                teclaEsquerda = true;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D) {
                teclaDireita = true;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
                teclaCima = true;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
                teclaBaixo = true;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_Q && jogo.getEstado() == EstadoJogo::JOGANDO) {
                ALLEGRO_MOUSE_STATE estadoMouse;
                al_get_mouse_state(&estadoMouse);
                jogo.processarTiro(converterParaCoordenadasLogicas(display, estadoMouse.x, estadoMouse.y));
            } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                rodando = false;
            }
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT || event.keyboard.keycode == ALLEGRO_KEY_A) {
                teclaEsquerda = false;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT || event.keyboard.keycode == ALLEGRO_KEY_D) {
                teclaDireita = false;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_UP || event.keyboard.keycode == ALLEGRO_KEY_W) {
                teclaCima = false;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN || event.keyboard.keycode == ALLEGRO_KEY_S) {
                teclaBaixo = false;
            }
        }


        if (redesenhando && al_is_event_queue_empty(event_queue)) {
            redesenhando = false;

            const float escalaX = static_cast<float>(al_get_display_width(display)) / static_cast<float>(LARGURA);
            const float escalaY = static_cast<float>(al_get_display_height(display)) / static_cast<float>(ALTURA);
            ALLEGRO_TRANSFORM transformacao;
            al_identity_transform(&transformacao);
            al_scale_transform(&transformacao, escalaX, escalaY);
            al_use_transform(&transformacao);

            al_clear_to_color(al_map_rgb(194, 229, 244));
            if (backgroundSheet) {
                al_draw_scaled_bitmap(backgroundSheet, 0, 0, al_get_bitmap_width(backgroundSheet), al_get_bitmap_height(backgroundSheet), 0, 0, LARGURA, ALTURA, 0);
            }
            jogo.desenhar(spritesHeroi, spritesItens, spriteGaivota);
            desenharHud(jogo, font);

            if (jogo.getEstado() != EstadoJogo::JOGANDO) {
                desenharMensagemFinal(jogo, font);
            }

            al_flip_display();
            al_identity_transform(&transformacao);
            al_use_transform(&transformacao);
            
        }
    }

    destruirSprites(spritesHeroi);
    destruirSprites(spritesItens);
    if (spriteGaivota) al_destroy_bitmap(spriteGaivota);
    if (itemSheet) al_destroy_bitmap(itemSheet);
    if (heroSheet) al_destroy_bitmap(heroSheet);
    if (gaivotaSheet) al_destroy_bitmap(gaivotaSheet);
    if (backgroundSheet) al_destroy_bitmap(backgroundSheet);
    if (somFundo) al_destroy_sample(somFundo);
    if (somTiro) al_destroy_sample(somTiro);
    if (somDanoArea) al_destroy_sample(somDanoArea);

    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_uninstall_audio();

    return 0;
}