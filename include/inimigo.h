#pragma once
#include "vetor2d.h"
#include <list>

class Projetil;

class Inimigo {
    public:
        Inimigo(const Vetor2D& posicaoInicial, int vidaMaxima, float alcanceMaximoProjetil);

        void atualizar(float deltaTime, const Vetor2D& alvo, std::list<Projetil>& listaProjetis);
        void desenhar() const;

        void receberDano(int dano);
        bool estaMorto() const;

        Vetor2D getPosicao() const;
        int getVida() const;

        int gerarMunicaoLargada() const;
        
    private:
        Vetor2D posicao;
        int vida;
        int vidaMaxima;

        float cronometroTiro = 0.0f;
        float intervaloEntreTiros = 1.5f; // Tempo em segundos entre tiros
        
};