#include "../include/inimigo.h"
#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>

#include <allegro5/allegro_image.h>

#include <cmath>

#include <random>

namespace {
int proximoIndiceSprite(int indiceAtual) {
    const int grupoBase = (indiceAtual >= 3) ? 3 : 0;
    const int posicaoGrupo = indiceAtual % 3;
    return grupoBase + ((posicaoGrupo + 1) % 3);
}
}

Inimigo::Inimigo(const Vetor2D& posicaoInicial, int vidaMaxima, float alcanceMaximoProjetil, int indiceSpriteInicial)
    : posicao(posicaoInicial), vida(vidaMaxima), vidaMaxima(vidaMaxima), alcanceMaximoProjetil(alcanceMaximoProjetil), velocidade(42.0f), indiceSprite(indiceSpriteInicial % 6) {}

void Inimigo::atualizar(float deltaTime, const Vetor2D& alvo, std::list<Projetil>& listaProjetis, ALLEGRO_SAMPLE* somTiro) {
    cronometroTiro += deltaTime;

    Vetor2D direcao = alvo - posicao;
    float distanciaAlvo = direcao.magnitude();

    if (distanciaAlvo > 1.0f) {
        direcao.normalizar();
        posicao = posicao + direcao * velocidade * deltaTime;
    }

    tempoAnimacao += deltaTime;
    if (tempoAnimacao >= 0.18f) {
        indiceSprite = proximoIndiceSprite(indiceSprite);
        tempoAnimacao = 0.0f;
    }

    if (cronometroTiro >= intervaloEntreTiros) {
        if (distanciaAlvo > 0.001f) {
            direcao.normalizar();
            static std::random_device rd;
            static std::mt19937 gerador(rd());
            std::uniform_int_distribution<int> sorteioSprite(0, 3);
            listaProjetis.emplace_back(posicao, direcao, 450.0f, alcanceMaximoProjetil, Dono::INIMIGO, sorteioSprite(gerador));
            if (somTiro) {
                al_play_sample(somTiro, 0.7f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
            }
            cronometroTiro = 0.0f;
        }
    }
}

void Inimigo::desenhar(ALLEGRO_BITMAP* spriteGaivota) const {
    if (spriteGaivota) {
        float scaleX = 1.15f;
        float scaleY = 1.15f;
        al_draw_scaled_bitmap(
            spriteGaivota,
            0,
            0,
            al_get_bitmap_width(spriteGaivota),
            al_get_bitmap_height(spriteGaivota),
            posicao.x - (al_get_bitmap_width(spriteGaivota) * scaleX) / 2.0f,
            posicao.y - (al_get_bitmap_height(spriteGaivota) * scaleY) / 2.0f,
            al_get_bitmap_width(spriteGaivota) * scaleX,
            al_get_bitmap_height(spriteGaivota) * scaleY,
            0);
        return;
    }

    float flap = (indiceSprite < 3) ? -1.0f : 1.0f;
    float wingOffsetY = 4.0f + flap * 4.0f;

    const ALLEGRO_COLOR corpo = al_map_rgb(250, 250, 250);
    const ALLEGRO_COLOR contorno = al_map_rgb(80, 80, 80);
    const ALLEGRO_COLOR bico = al_map_rgb(255, 168, 45);
    const ALLEGRO_COLOR asa = al_map_rgb(230, 230, 230);

    al_draw_filled_ellipse(posicao.x, posicao.y, 18.0f, 12.0f, corpo);
    al_draw_filled_triangle(posicao.x - 3.0f, posicao.y + 2.0f, posicao.x - 28.0f, posicao.y - wingOffsetY, posicao.x - 10.0f, posicao.y + 12.0f, asa);
    al_draw_filled_triangle(posicao.x + 3.0f, posicao.y + 2.0f, posicao.x + 28.0f, posicao.y - wingOffsetY, posicao.x + 10.0f, posicao.y + 12.0f, asa);
    al_draw_filled_triangle(posicao.x + 15.0f, posicao.y - 2.0f, posicao.x + 26.0f, posicao.y, posicao.x + 15.0f, posicao.y + 2.0f, bico);
    al_draw_ellipse(posicao.x, posicao.y, 18.0f, 12.0f, contorno, 1.5f);
}

void Inimigo::receberDano(int dano) {
    vida -= dano;
    if (vida < 0) {
        vida = 0;
    }
}

bool Inimigo::estaMorto() const {
    return vida <= 0;
}

Vetor2D Inimigo::getPosicao() const {
    return posicao;
}

int Inimigo::getVida() const {
    return vida;
}

int Inimigo::gerarMunicaoLargada() const {
    return 10;
}

