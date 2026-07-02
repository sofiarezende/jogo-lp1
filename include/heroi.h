#pragma once
#include "vetor2d.h"
#include <list>
#include <allegro5/allegro.h> 

class Projetil;

class Heroi {
public:
    Heroi(const Vetor2D& posicaoInicial, int vidaMaxima, int municaoInicial, float alcanceMaximoProjetil);
    ~Heroi(); 

    void definirDestino(const Vetor2D& destino);
    void atualizar(float deltaTime);
    void desenhar() const;

    void receberDano(int dano);
    bool estaVivo() const;
    
    void atirar(Vetor2D& posicaoMouse, std::list<Projetil>& listaProjetis);

    void ganharMunicao(int quantidade);
    int getMunicao() const;

    Vetor2D getPosicao() const;
    int getVida() const;

private:
    Vetor2D posicao;
    Vetor2D destino;
    int vida;
    int vidaMaxima;
    int municao;
    float alcanceMaximoProjetil;

    ALLEGRO_BITMAP* imagemTartaruga;
    float angulo; 
};