#pragma once
#include "jogo.h"
#include "heroi.h"
#include "base.h"
#include "inimigo.h"
#include "projetil.h"
#include "municao.h"
#include <list>
#include <queue>

enum class EstadoJogo { JOGANDO, VITORIA, DERROTA };

class Jogo {
public:
    Jogo(int larguraMapa, int alturaMapa);

    void processarClique(Vetor2D posicaoMouse);
    void processarTiro(Vetor2D posicaoMouse);
    void atualizar(float dt);
    void desenhar() const;

    EstadoJogo getEstado() const;

private:
    int larguraMapa, alturaMapa;

    Heroi heroi;
    Base base;
    std::list<Inimigo> inimigos;
    std::list<Projetil> projeteis;
    std::queue<Municao> municoesNoChao; // ou o que você decidiu no Desafio 13

    float cronometroSpawn = 0.0f;
    float intervaloSpawnAtual = 3.0f;

    float tempoSobrevivido = 0.0f;
    float tempoParaVencer = 120.0f; // 2 minutos, ajuste ao gosto

    EstadoJogo estado = EstadoJogo::JOGANDO;

    // TODO: métodos privados de apoio, ex:
    void atualizarInimigos(float dt);
    void atualizarProjeteis(float dt);
    void atualizarMunicoes(float dt);
    void verificarColisoes();
    void verificarSpawnDeInimigos(float dt);
    void verificarCondicoesDeFim(float dt);
};