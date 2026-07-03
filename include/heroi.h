#pragma once
#include "vetor2d.h"
#include <list>
#include <vector>

struct ALLEGRO_BITMAP;

class Projetil;

class Heroi {
public:
    Heroi(const Vetor2D& posicaoInicial, int vidaMaxima, int municaoInicial, float alcanceMaximoProjetil);

    void definirDestino(const Vetor2D& destino);
    void atualizar(float deltaTime);
    void moverComDirecao(const Vetor2D& direcao, float deltaTime, float larguraMapa, float alturaMapa);
    void desenhar(const std::vector<ALLEGRO_BITMAP*>& spritesHeroi) const;

    void receberDano(int dano);
    void curar(int quantidade);
    bool estaVivo() const;
    
    bool atirar(const Vetor2D& posicaoMouse, std::list<Projetil>& listaProjetis);

    void ganharMunicao(int quantidade);
    int getMunicao() const;
    int getVidaMaxima() const;

    Vetor2D getPosicao() const;
    int getVida() const;

private:
    Vetor2D posicao;
    Vetor2D destino;
    int vida;
    int vidaMaxima;
    int municao;
    float alcanceMaximoProjetil;

    float angulo; 
};