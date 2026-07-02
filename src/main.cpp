#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>
#include <random>
#include <iostream>
#include <list>

#include "../include/vetor2d.h"
#include "../include/Jogo.h"


const int LARGURA = 800;
const int ALTURA = 600;
const float FPS = 60.0;





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

    Jogo jogo(LARGURA, ALTURA);

    bool rodando = true;
    bool redesenhando = true;

    while(rodando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            float deltaTime = 1.0f / FPS;
            
            jogo.atualizar(deltaTime);

            redesenhando = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = false;
        }


        if (redesenhando && al_is_event_queue_empty(event_queue)) {
            redesenhando = false;

            al_clear_to_color(al_map_rgb(255, 255, 255));
            jogo.desenhar();
            al_flip_display();
            
        }
    }

    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}