#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <vector>

Projetil::Projetil(const Vetor2D& posicaoInicial, const Vetor2D& direcao, float velocidade, float alcanceMaximo, Dono dono, int indiceSpriteVisual)
    : posicao(posicaoInicial), posicaoInicial(posicaoInicial), direcao(direcao), velocidade(velocidade), alcanceMaximo(alcanceMaximo), dono(dono), indiceSpriteVisual(indiceSpriteVisual) {}

void Projetil::atualizar(float deltaTime) {
    posicao = posicao + direcao * velocidade * deltaTime;
}

void Projetil::desenhar(const std::vector<ALLEGRO_BITMAP*>& spritesItens) const {
    if (indiceSpriteVisual >= 0 && indiceSpriteVisual < static_cast<int>(spritesItens.size()) && spritesItens[indiceSpriteVisual]) {
        al_draw_scaled_bitmap(
            spritesItens[indiceSpriteVisual],
            0, 0,
            al_get_bitmap_width(spritesItens[indiceSpriteVisual]),
            al_get_bitmap_height(spritesItens[indiceSpriteVisual]),
            posicao.x - 14.0f,
            posicao.y - 14.0f,
            28.0f,
            28.0f,
            0);
        return;
    }

    ALLEGRO_COLOR cor = (dono == Dono::HEROI)
        ? al_map_rgb(255, 255, 0)   // amarelo = tiro do herói
        : al_map_rgb(255, 100, 0);  // laranja = tiro do inimigo

    al_draw_filled_circle(posicao.x, posicao.y, 7.5f, cor);
}

bool Projetil::ultrapassouAlcance() const {
    float distanciaPercorrida = (posicao - posicaoInicial).magnitude();
    return distanciaPercorrida > alcanceMaximo;
}

Vetor2D Projetil::getPosicao() const {
    return posicao;
}

Dono Projetil::getDono() const {
    return dono;
}