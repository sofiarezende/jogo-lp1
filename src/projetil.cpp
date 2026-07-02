#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Projetil::Projetil(const Vetor2D& posicaoInicial, const Vetor2D& direcao, float velocidade, float alcanceMaximo, Dono dono)
    : posicao(posicaoInicial), posicaoInicial(posicaoInicial), direcao(direcao), velocidade(velocidade), alcanceMaximo(alcanceMaximo), dono(dono) {}

void Projetil::atualizar(float deltaTime) {
    posicao = posicao + direcao * velocidade * deltaTime;
}

void Projetil::desenhar() const {
    ALLEGRO_COLOR cor = (dono == Dono::HEROI)
        ? al_map_rgb(255, 255, 0)   // amarelo = tiro do herói
        : al_map_rgb(255, 100, 0);  // laranja = tiro do inimigo

    al_draw_filled_circle(posicao.x, posicao.y, 5.0f, cor);
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