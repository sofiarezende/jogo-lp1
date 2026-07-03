#include "jogo.h"
#include "heroi.h"
#include "base.h"
#include "inimigo.h"
#include "projetil.h"
#include "municao.h"

#include <list>
#include <vector>
#include <cstdio>
#include <random>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>

namespace {
    constexpr float RAIO_HEROI = 14.0f;
    constexpr float RAIO_INIMIGO = 24.0f;
    constexpr float RAIO_PROJETIL = 5.0f;
    constexpr float CHANCE_QUEDA_MUNICAO = 0.30f;
    constexpr float LARGURA_BASE_FRACAO = 0.25f;
    constexpr float ALTURA_BASE_FRACAO = 0.25f;

    float calcularAlcanceTela(float largura, float altura) {
        return std::hypot(largura, altura);
    }

    bool pontoDentroDoCirculo(const Vetor2D& ponto, const Vetor2D& centro, float raio) {
        return distancia(ponto, centro) <= raio;
    }

    bool circulosColidem(const Vetor2D& a, float raioA, const Vetor2D& b, float raioB) {
        return distancia(a, b) <= (raioA + raioB);
    }
}

Jogo::Jogo(int larguraMapa, int alturaMapa, ALLEGRO_SAMPLE* somTiro, ALLEGRO_SAMPLE* somDanoArea)
    : larguraMapa(larguraMapa), alturaMapa(alturaMapa),
    heroi(Vetor2D(larguraMapa / 2.0f, alturaMapa / 2.0f), 100, 15, calcularAlcanceTela(larguraMapa, alturaMapa)),
    base(Vetor2D((larguraMapa - larguraMapa * LARGURA_BASE_FRACAO) / 2.0f,
                 (alturaMapa - alturaMapa * ALTURA_BASE_FRACAO) / 2.0f),
         larguraMapa * LARGURA_BASE_FRACAO,
         alturaMapa * ALTURA_BASE_FRACAO,
         100),
    somTiro(somTiro),
    somDanoArea(somDanoArea) {
}

Jogo::~Jogo() {
}

void Jogo::processarClique(Vetor2D posicaoMouse) {
    heroi.definirDestino(posicaoMouse);
}

void Jogo::processarTiro(Vetor2D posicaoMouse) {
    if (base.contemPonto(heroi.getPosicao())) {
        return;
    }

    if (heroi.atirar(posicaoMouse, projeteis)) {
        tocarSomTiro();
    }
}

void Jogo::processarMovimentoTeclado(const Vetor2D& direcao, float dt) {
    heroi.moverComDirecao(direcao, dt, static_cast<float>(larguraMapa), static_cast<float>(alturaMapa));
}

void Jogo::reiniciar() {
    *this = Jogo(larguraMapa, alturaMapa);
}

void Jogo::atualizarInimigos(float dt) {
    auto it = inimigos.begin();
    
    while (it != inimigos.end()) {
        it->atualizar(dt, heroi.getPosicao(), projeteis, somTiro);

        if (it->estaMorto()) {
            static std::random_device rd;
            static std::mt19937 gerador(rd());
            std::uniform_real_distribution<float> sorteioChance(0.0f, 1.0f);
            if (sorteioChance(gerador) <= CHANCE_QUEDA_MUNICAO) {
                std::uniform_int_distribution<int> sorteioQuantidade(5, 12);
                municoesNoChao.emplace_back(it->getPosicao(), sorteioQuantidade(gerador), 12.0f);
            }
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

void Jogo::atualizarMunicoes(float dt) {
    for (auto it = municoesNoChao.begin(); it != municoesNoChao.end(); ) {
        it->atualizar(dt);
        if (it->estaExpirada()) {
            it = municoesNoChao.erase(it);
        } else {
            ++it;
        }
    }
}

void Jogo::atualizarZonaSegura(float dt) {
    if (base.contemPonto(heroi.getPosicao())) {
        cronometroZonaSegura += dt;
        if (cronometroZonaSegura >= intervaloCuraZonaSegura) {
            heroi.curar(1);
            cronometroZonaSegura = 0.0f;
        }
    } else {
        cronometroZonaSegura = 0.0f;
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
        static std::random_device rd;
        static std::mt19937 gerador(rd());
        std::uniform_int_distribution<int> sorteioSprite(0, 5);
        inimigos.emplace_back(posNova, /*vidaMaxima=*/30, /*alcanceMaximoProjetil=*/calcularAlcanceTela(larguraMapa, alturaMapa), sorteioSprite(gerador));
        cronometroSpawn = 0.0f;

        // aumenta a dificuldade aos poucos
        intervaloSpawnAtual = std::max(0.8f, intervaloSpawnAtual * 0.97f);
    }
}

void Jogo::verificarColisoes() {
    bool heroiNaZonaSegura = base.contemPonto(heroi.getPosicao());

    for (auto proj = projeteis.begin(); proj != projeteis.end(); ) {
        bool removerProjetil = false;

        if (proj->getDono() == Dono::HEROI) {
            for (auto inimigo = inimigos.begin(); inimigo != inimigos.end(); ++inimigo) {
                if (circulosColidem(proj->getPosicao(), RAIO_PROJETIL, inimigo->getPosicao(), RAIO_INIMIGO)) {
                    inimigo->receberDano(20);
                    removerProjetil = true;
                    break;
                }
            }

            if (!removerProjetil && base.contemPonto(proj->getPosicao())) {
                removerProjetil = true;
            }
        } else {
            if (!heroiNaZonaSegura && circulosColidem(proj->getPosicao(), RAIO_PROJETIL, heroi.getPosicao(), RAIO_HEROI)) {
                heroi.receberDano(10);
                removerProjetil = true;
            } else if (base.contemPonto(proj->getPosicao())) {
                base.receberDano(10);
                tocarSomDanoArea();
                removerProjetil = true;
            }
        }

        if (removerProjetil) {
            proj = projeteis.erase(proj);
        } else {
            ++proj;
        }
    }

    for (auto it = municoesNoChao.begin(); it != municoesNoChao.end(); ) {
        if (circulosColidem(it->getPosicao(), 10.0f, heroi.getPosicao(), RAIO_HEROI)) {
            heroi.ganharMunicao(it->getQuantidade());
            it = municoesNoChao.erase(it);
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
    atualizarMunicoes(dt);
    atualizarZonaSegura(dt);

    verificarColisoes();
    verificarSpawnDeInimigos(dt);
    verificarCondicoesDeFim(dt);
}

void Jogo::desenhar(const std::vector<ALLEGRO_BITMAP*>& spritesHeroi,
                    const std::vector<ALLEGRO_BITMAP*>& spritesItens,
                    ALLEGRO_BITMAP* spriteGaivota) const {
    base.desenhar();

    for (const Inimigo& inimigo : inimigos) {
        inimigo.desenhar(spriteGaivota);
    }

    for (const Projetil& projetil : projeteis) {
        projetil.desenhar(spritesItens);
    }

    for (const Municao& municao : municoesNoChao) { 
        municao.desenhar(spritesItens);
    }

    heroi.desenhar(spritesHeroi);

    if (estado != EstadoJogo::JOGANDO) {
        al_draw_filled_rectangle(0, 0, larguraMapa, alturaMapa, al_map_rgba(0, 0, 0, 180));
    }
}

EstadoJogo Jogo::getEstado() const {
    return estado;
}

float Jogo::getTempoSobrevivido() const {
    return tempoSobrevivido;
}

float Jogo::getTempoParaVencer() const {
    return tempoParaVencer;
}

int Jogo::getVidaHeroi() const {
    return heroi.getVida();
}

int Jogo::getVidaHeroiMaxima() const {
    return heroi.getVidaMaxima();
}

int Jogo::getMunicaoHeroi() const {
    return heroi.getMunicao();
}

int Jogo::getVidaBase() const {
    return base.getVida();
}

int Jogo::getVidaBaseMaxima() const {
    return base.getVidaMaxima();
}

void Jogo::verificarCondicoesDeFim(float dt) {
    tempoSobrevivido += dt;

    if (!heroi.estaVivo()) {
        estado = EstadoJogo::DERROTA;
        return;
    }

    if (base.estaDestruida()) {
        estado = EstadoJogo::DERROTA;
        return;
    }

    if (tempoSobrevivido >= tempoParaVencer) {
        estado = EstadoJogo::VITORIA;
    }
}

void Jogo::tocarSomTiro() const {
    if (somTiro) {
        al_play_sample(somTiro, 0.8f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}

void Jogo::tocarSomDanoArea() const {
    if (somDanoArea) {
        al_play_sample(somDanoArea, 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
}