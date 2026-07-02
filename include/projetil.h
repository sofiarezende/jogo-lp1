#pragma once
#include "vetor2d.h"

enum class Dono {
    HEROI,
    INIMIGO
};

class Projetil {
public:
    Projetil(const Vetor2D& posicaoInicial, const Vetor2D& direcao, float velocidade, float alcanceMaximo, Dono dono);

    void atualizar(float deltaTime);
    void desenhar() const;

    bool ultrapassouAlcance() const;
    bool foiCriadorPor(Dono dono) const;

    Vetor2D getPosicao() const;

    Dono getDono() const;

private:
    Vetor2D posicao;
    Vetor2D posicaoInicial;
    Vetor2D direcao;

    float velocidade;
    float alcanceMaximo;

    Dono dono;
};