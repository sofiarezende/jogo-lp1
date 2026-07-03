#include "municao.h"
#include "vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <vector>
#include <cstdio>
#include <iostream>
#include <random>

const int ITEM_LARGURA = 16;
const int ITEM_ALTURA = 16;
const int ITEM_COLUNAS = 3;
const int ITEM_LINHAS = 2;

Municao::Municao(const Vetor2D& posicao, int quantidade, float tempoDeVida)
    : posicao(posicao), quantidade(quantidade), tempoRestante(tempoDeVida) {
    static std::random_device rd;
    static std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> sorteio(0, 3); // 4 sprites (0 a 3)
    indiceSprite = sorteio(gerador);
}

int Municao::getIndiceSprite() const {
    return indiceSprite;
}

void Municao::atualizar(float deltaTime) {
    tempoRestante -= deltaTime;
}

void Municao::desenhar(const std::vector<ALLEGRO_BITMAP*>& sprites) const {
    ALLEGRO_BITMAP* sprite = sprites[indiceSprite];
    al_draw_bitmap(sprite, posicao.x - ITEM_LARGURA / 2, posicao.y - ITEM_ALTURA / 2, 0);
}

bool Municao::estaExpirada() const {
    return tempoRestante <= 0.0f;
}

Vetor2D Municao::getPosicao() const {
    return posicao;
}

