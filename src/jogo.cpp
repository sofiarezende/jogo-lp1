#include "../include/jogo.h"
#include "../include/Heroi.h"
#include "../include/Base.h"
#include "../include/Inimigo.h"
#include "../include/Projetil.h"
#include "../include/Municao.h"

#include <list>
#include <cstdio>
#include <random>
#include <iostream>
#include <queue>

Jogo::Jogo(int larguraMapa, int alturaMapa)
    : larguraMapa(larguraMapa), alturaMapa(alturaMapa),
      heroi(Vetor2D(larguraMapa / 2.0f, alturaMapa / 2.0f), 100, 0, 300.0f),
      base(Vetor2D(larguraMapa / 2.0f - 50.0f, alturaMapa - 100.0f), 100, 80, 100) {
    // Inicialização de outros membros se necessário
}

void Jogo::processarClique(Vetor2D posicaoMouse) {
    heroi.definirDestino(posicaoMouse);
}

void Jogo::processarTiro(Vetor2D posicaoMouse) {
    heroi.atirar(posicaoMouse, projeteis);
}

void Jogo::atualizarInimigos(float dt) {
    auto it = inimigos.begin();
    
    while (it != inimigos.end()) {
        it->atualizar(dt, base.getPosicao(), projeteis);

        if (it->estaMorto()) {
            it = inimigos.erase(it);
        } else {
            ++it;
        }
    }
}

void Jogo::atualizarProjeteis(float dt) {
    for (auto it = projeteis.begin(); it != projeteis.end();) {
        it->atualizar(dt);

        if (it->ultrapassouAlcance()) {
            it = projeteis.erase(it);
        } else {
            ++it;
        }
    }
}

Vetor2D posicaoAleatoriaNaBorda(float largura, float altura) {
    static std::random_device rd;
    static std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> sorteioLado(0, 3);
    std::uniform_real_distribution<float> sorteioX(0.0f, largura);
    std::uniform_real_distribution<float> sorteioY(0.0f, altura);

    int lado = sorteioLado(gerador); // 0=topo, 1=direita, 2=baixo, 3=esquerda

    switch (lado) {
        case 0: return {sorteioX(gerador), 0.0f};
        case 1: return {largura, sorteioY(gerador)};
        case 2: return {sorteioX(gerador), altura};
        default: return {0.0f, sorteioY(gerador)};
    }
}

void Jogo::verificarSpawnDeInimigos(float dt) {
    cronometroSpawn += dt;

    if (cronometroSpawn >= intervaloSpawnAtual) {
        Vetor2D posNova = posicaoAleatoriaNaBorda(larguraMapa, alturaMapa);
        inimigos.emplace_back(posNova, /*vidaMaxima=*/30, /*alcanceMaximoProjetil=*/100.0f);
        cronometroSpawn = 0.0f;

        // aumenta a dificuldade aos poucos
        intervaloSpawnAtual = std::max(0.8f, intervaloSpawnAtual * 0.97f);
    }
}

void Jogo::verificarColisoes() {
    for (auto it = projeteis.begin(); it != projeteis.end();) {
        if (it->getDono() == Dono::INIMIGO && base.contemPonto(it->getPosicao())) {
            base.receberDano(10); 
            it = projeteis.erase(it);
        } else {
            ++it;
        }
    }
}

void Jogo::atualizar(float dt) {
    if (estado != EstadoJogo::JOGANDO) return;

    heroi.atualizar(dt);
    base.atualizar(dt);

    atualizarInimigos(dt);
    atualizarProjeteis(dt);
    ///atualizarMunicoes(dt);

    verificarColisoes();
    verificarSpawnDeInimigos(dt);
    ///verificarCondicoesDeFim(dt);
}

void Jogo::desenhar() const {
    base.desenhar();

    for (const Inimigo& inimigo : inimigos) {
        inimigo.desenhar();
    }

    for (const Projetil& projetil : projeteis) {
        projetil.desenhar();
    }

    heroi.desenhar();
}

EstadoJogo Jogo::getEstado() const {
    return estado;
}