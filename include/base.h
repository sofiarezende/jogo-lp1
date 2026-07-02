#pragma once
#include "vetor2d.h"

//todo: separar a interface da implementação, talvez em base.cpp
//todo: implementar as funções (apenas atualizar esta implementada)

class Base {
public:
    Base(const Vetor2D& posicao, float largura, float altura, int vidaMaxima);

    void atualizar(float deltaTime) {
        tempoRegeneracao += deltaTime;

        if(tempoRegeneracao >= intervaloRegeneracao) {
            if (vida < vidaMaxima) {
                vida++;
            }
            tempoRegeneracao = 0.0f;
        }

    }

    void desenhar() const;

    void receberDano(int dano);
    bool estaDestruida() const;
    bool contemPonto(const Vetor2D& ponto) const;

    Vetor2D getPosicao() const;

private:
    Vetor2D posicao;
    float largura, altura;
    int vida;
    int vidaMaxima;

    // Variáveis para controle da regeneração de vida
    float tempoRegeneracao = 0.0f;
    float intervaloRegeneracao = 2.0f; // Tempo em segundos para regenerar vida
};