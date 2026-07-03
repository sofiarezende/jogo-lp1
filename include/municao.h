#pragma once
#include "vetor2d.h"
#include <list>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

class Municao {
public:
    Municao(const Vetor2D& posicao, int quantidade, float tempoDeVida);

    void atualizar(float deltaTime);
    void desenhar(const std::vector<ALLEGRO_BITMAP*>& sprites) const;

    bool estaExpirada() const;
    Vetor2D getPosicao() const;
    int getQuantidade() const;
    int getIndiceSprite() const;
    void setIndiceSprite(int novoIndice);

private:
    Vetor2D posicao;
    int quantidade;
    float tempoRestante;
    int indiceSprite;
};