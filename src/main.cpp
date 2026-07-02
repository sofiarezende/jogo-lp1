#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>
#include <random>
#include <iostream>
#include <list>

#include "../include/base.h"
#include "../include/vetor2d.h"
#include "../include/inimigo.h"
#include "../include/projetil.h"


const int LARGURA = 800;
const int ALTURA = 600;
const float FPS = 60.0;


Vetor2D posicaoAleatoriaNaBorda(float largura, float altura) {
    static std::random_device rd;
    static std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> sorteioLado(0, 3);
    std::uniform_real_distribution<float> sorteioX(0.0f, largura);
    std::uniform_real_distribution<float> sorteioY(0.0f, altura);

    int lado = sorteioLado(gerador); // 0=topo, 1=direita, 2=baixo, 3=esquerda

    switch (lado) {
        case 0: return {sorteioX(gerador), 0.0f};
        case 1: return {largura, sorteioY(gerador)};
        case 2: return {sorteioX(gerador), altura};
        default: return {0.0f, sorteioY(gerador)};
    }
}


int main(int argc, char **argv) {
    if(!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_DISPLAY* display = al_create_display(LARGURA, ALTURA);
    al_set_window_position(display, 200, 200);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

    Vetor2D posicaoBase = {350.0f, 250.0f};
    Base base(posicaoBase, 100, 80, 100); 

    std::list<Projetil> projeteis;
    std::list<Inimigo> inimigos;

    float cronometroSpawn = 0.0f;
    float intervaloSpawnAtual = 3.0f;   // começa spawnando a cada 3s
    const float intervaloSpawnMinimo = 0.8f; // nunca fica mais rápido que isso

    bool rodando = true;
    bool redesenhando = true;

    while(rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            float deltaTime = 1.0f / FPS;
            
            base.atualizar(deltaTime);

            cronometroSpawn += deltaTime;
            if (cronometroSpawn >= intervaloSpawnAtual) {
                Vetor2D posNova = posicaoAleatoriaNaBorda(LARGURA, ALTURA);
                inimigos.emplace_back(posNova, /*vidaMaxima=*/30, /*alcanceMaximoProjetil=*/100.0f);
                cronometroSpawn = 0.0f;

                // aumenta a dificuldade aos poucos
                intervaloSpawnAtual = std::max(intervaloSpawnMinimo, intervaloSpawnAtual * 0.97f);
            }

            // --- atualizar inimigos existentes ---
            for (Inimigo& inimigo : inimigos) {
                inimigo.atualizar(deltaTime, base.getPosicao(), projeteis); // ver nota abaixo
            }

            // --- remover inimigos mortos ---
            inimigos.remove_if([](const Inimigo& i) { return i.estaMorto(); });

            for (Projetil& projetil : projeteis) {
                projetil.atualizar(deltaTime);
            }

            projeteis.remove_if([&base](const Projetil& p) {
                if (p.getDono() == Dono::INIMIGO && base.contemPonto(p.getPosicao())) {
                    base.receberDano(10); // ajuste o valor de dano ao gosto
                    return true; // remove o projétil da lista
                }
                return false;
            });

            projeteis.remove_if([](const Projetil& p) { return p.ultrapassouAlcance(); });



            redesenhando = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }


        if (redesenhando && al_is_event_queue_empty(event_queue)) {
            redesenhando = false;

            al_clear_to_color(al_map_rgb(255, 255, 255));
            base.desenhar();

            for (const Inimigo& inimigo : inimigos) {
                inimigo.desenhar();
            }

            for (const Projetil& projetil : projeteis) {
                projetil.desenhar();
            }

            al_flip_display();
            
        }
    }

    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}