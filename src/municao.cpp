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

void Municao::setIndiceSprite(int novoIndice) {
    indiceSprite = novoIndice;
}

int Municao::getIndiceSprite() const {
    return indiceSprite;
}

int Municao::getQuantidade() const {
    return quantidade;
}

void Municao::atualizar(float deltaTime) {
    tempoRestante -= deltaTime;
}

void Municao::desenhar(const std::vector<ALLEGRO_BITMAP*>& sprites) const {
    if (!sprites.empty() && indiceSprite >= 0 && indiceSprite < static_cast<int>(sprites.size()) && sprites[indiceSprite]) {
        al_draw_scaled_bitmap(
            sprites[indiceSprite],
            0, 0,
            al_get_bitmap_width(sprites[indiceSprite]),
            al_get_bitmap_height(sprites[indiceSprite]),
            posicao.x - 12.0f,
            posicao.y - 12.0f,
            24.0f,
            24.0f,
            0);
        return;
    }

    al_draw_filled_circle(posicao.x, posicao.y, 8.0f, al_map_rgb(255, 255, 0));
}

bool Municao::estaExpirada() const {
    return tempoRestante <= 0.0f;
}

Vetor2D Municao::getPosicao() const {
    return posicao;
}

