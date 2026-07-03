#pragma once
#include "vetor2d.h"
#include <list>

struct ALLEGRO_BITMAP;
struct ALLEGRO_SAMPLE;

class Projetil;

class Inimigo {
    public:
        Inimigo(const Vetor2D& posicaoInicial, int vidaMaxima, float alcanceMaximoProjetil, int indiceSpriteInicial = 0);

        void atualizar(float deltaTime, const Vetor2D& alvo, std::list<Projetil>& listaProjetis, ALLEGRO_SAMPLE* somTiro = nullptr);
        void desenhar(ALLEGRO_BITMAP* spriteGaivota) const;

        void receberDano(int dano);
        bool estaMorto() const;

        Vetor2D getPosicao() const;
        int getVida() const;

        int gerarMunicaoLargada() const;
        
    private:
        Vetor2D posicao;
        int vida;
        int vidaMaxima;
        float alcanceMaximoProjetil;
        float velocidade;
        int indiceSprite;
        float tempoAnimacao = 0.0f;

        float cronometroTiro = 0.0f;
        float intervaloEntreTiros = 1.5f; // Tempo em segundos entre tiros
        
};