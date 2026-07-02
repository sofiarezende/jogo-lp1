#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main() {
    // Inicializa o Allegro
    if (!al_init()) {
        return -1;
    }

    // Inicializa o addon de primitivas
    if (!al_init_primitives_addon()) {
        return -1;
    }

    // Cria uma janela 800x600
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);

    if (!display) {
        return -1;
    }

    // Muda o título da janela
    al_set_window_title(display, "Teste Allegro 5");

    // Pinta o fundo de azul
    al_clear_to_color(al_map_rgb(30, 144, 255));

    // Desenha um círculo branco
    al_draw_filled_circle(400, 300, 50, al_map_rgb(255, 255, 255));

    // Atualiza a tela
    al_flip_display();

    // Espera 5 segundos
    al_rest(5.0);

    // Libera a memória
    al_destroy_display(display);

    return 0;
}