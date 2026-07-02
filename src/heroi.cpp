#include "include/heroi.h"
#include <allegro5/allegro.h>           
#include <allegro5/allegro_image.h>     
#include <allegro5/allegro_primitives.h>

void Heroi::desenhar() const {
    if (imagemTartaruga) {
        float frameLargura = 96.0f;
        float frameAltura = 48.0f;

        // Cria o recorte da primeira tartaruga (coluna 0, linha 0)
        ALLEGRO_BITMAP* subTartaruga = al_create_sub_bitmap(imagemTartaruga, 0, 0, frameLargura, frameAltura);

        if (subTartaruga) {
            // Desenha o recorte rotacionando pelo centro dele
            al_draw_scaled_rotated_bitmap(subTartaruga, frameLargura / 2.0f, frameAltura / 2.0f, posicao.x, posicao.y, 1.0f, 1.0f, angulo, 0);

            // Destrói o sub-bitmap temporário para limpar a memória
            al_destroy_bitmap(subTartaruga);
        }
    } else {
        // Fallback caso a imagem dê erro
        al_draw_filled_circle(posicao.x, posicao.y, 15, al_map_rgb(0, 255, 0));
    }
}