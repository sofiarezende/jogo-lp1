#include "../include/heroi.h"
#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>           
#include <allegro5/allegro_primitives.h>

#include <vector>

#include <iostream>
#include <cmath>
#include <random>

Heroi::Heroi(const Vetor2D& posicaoInicial, int vidaMaxima, int municaoInicial, float alcanceMaximoProjetil)
    : posicao(posicaoInicial), destino(posicaoInicial), vida(vidaMaxima), vidaMaxima(vidaMaxima), municao(municaoInicial), alcanceMaximoProjetil(alcanceMaximoProjetil), angulo(0.0f) {}

void Heroi::desenhar(const std::vector<ALLEGRO_BITMAP*>& spritesHeroi) const {
    if (!spritesHeroi.empty() && spritesHeroi[0]) {
        int indiceSprite = 0;
        if (spritesHeroi.size() >= 3) {
            indiceSprite = static_cast<int>(std::fmod(std::fabs(angulo) * 10.0f, 3.0f));
        }

        ALLEGRO_BITMAP* sprite = spritesHeroi[indiceSprite];
        if (sprite) {
            al_draw_scaled_rotated_bitmap(
                sprite,
                al_get_bitmap_width(sprite) / 2.0f,
                al_get_bitmap_height(sprite) / 2.0f,
                posicao.x,
                posicao.y,
                1.45f,
                1.45f,
                angulo + (ALLEGRO_PI * 0.5f),
                0);
            return;
        }
    }

    const ALLEGRO_COLOR corpo = al_map_rgb(46, 134, 88);
    const ALLEGRO_COLOR contorno = al_map_rgb(20, 54, 37);
    const ALLEGRO_COLOR casco = al_map_rgb(94, 155, 89);

    al_draw_filled_ellipse(posicao.x, posicao.y, 30.0f, 22.0f, casco);
    al_draw_filled_circle(posicao.x, posicao.y, 13.0f, corpo);
    al_draw_ellipse(posicao.x, posicao.y, 30.0f, 22.0f, contorno, 2.0f);

    Vetor2D ponta(posicao.x + std::cos(angulo) * 32.0f, posicao.y + std::sin(angulo) * 32.0f);
    al_draw_line(posicao.x, posicao.y, ponta.x, ponta.y, al_map_rgb(255, 255, 255), 2.0f);
}

void Heroi::definirDestino(const Vetor2D& destino) {
    this->destino = destino;
}

void Heroi::atualizar(float deltaTime) {
    Vetor2D direcao = destino - posicao;
    float distancia = direcao.magnitude();

    if (distancia > 1.0f) {
        direcao.normalizar();
        float velocidade = 200.0f;
        float passo = velocidade * deltaTime;

        if (passo >= distancia) {
            posicao = destino;
        } else {
            posicao = posicao + direcao * passo;
        }

        angulo = std::atan2(direcao.y, direcao.x);
    }
}

void Heroi::moverComDirecao(const Vetor2D& direcaoEntrada, float deltaTime, float larguraMapa, float alturaMapa) {
    if (direcaoEntrada.magnitude() <= 0.001f) {
        return;
    }

    Vetor2D direcao = direcaoEntrada;
    direcao.normalizar();

    float velocidade = 220.0f;
    Vetor2D novaPosicao = posicao + direcao * velocidade * deltaTime;

    if (novaPosicao.x < 0.0f) novaPosicao.x = 0.0f;
    if (novaPosicao.y < 0.0f) novaPosicao.y = 0.0f;
    if (novaPosicao.x > larguraMapa) novaPosicao.x = larguraMapa;
    if (novaPosicao.y > alturaMapa) novaPosicao.y = alturaMapa;

    posicao = novaPosicao;
    destino = posicao;
    angulo = std::atan2(direcao.y, direcao.x);
}

void Heroi::receberDano(int dano) {
    vida -= dano;
    if (vida < 0) {
        vida = 0;
    }
}

void Heroi::curar(int quantidade) {
    vida += quantidade;
    if (vida > vidaMaxima) {
        vida = vidaMaxima;
    }
}

bool Heroi::estaVivo() const {
    return vida > 0;
}

bool Heroi::atirar(const Vetor2D& posicaoMouse, std::list<Projetil>& listaProjetis) {
    if (municao <= 0) {
        return false;
    }

    Vetor2D direcao = posicaoMouse - posicao;
    if (direcao.magnitude() <= 0.001f) {
        return false;
    }

    direcao.normalizar();

    static std::random_device rd;
    static std::mt19937 gerador(rd());
    std::uniform_int_distribution<int> sorteioSprite(0, 3);

    listaProjetis.emplace_back(posicao, direcao, 500.0f, alcanceMaximoProjetil, Dono::HEROI, sorteioSprite(gerador));
    municao--;
    return true;
}

void Heroi::ganharMunicao(int quantidade) {
    municao += quantidade;
}

int Heroi::getMunicao() const {
    return municao;
}

int Heroi::getVidaMaxima() const {
    return vidaMaxima;
}

Vetor2D Heroi::getPosicao() const {
    return posicao;
}

int Heroi::getVida() const {
    return vida;
}
