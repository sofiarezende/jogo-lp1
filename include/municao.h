#pragma once
#include "vetor2d.h"

class Municao {
public:
    Municao(const Vetor2D& posicao, int quantidade, float tempoDeVida);

    void atualizar(float deltaTime);
    void desenhar() const;

    bool estaExpirada() const;
    Vetor2D getPosicao() const;
    int getQuantidade() const;

private:
    Vetor2D posicao;
    int quantidade;
    float tempoRestante;
};