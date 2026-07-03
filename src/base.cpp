#include "../include/base.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Base::Base(const Vetor2D& posicao, float largura, float altura, int vidaMaxima)
    : posicao(posicao), largura(largura), altura(altura), vida(vidaMaxima), vidaMaxima(vidaMaxima) {
        
    }

void Base::atualizar(float deltaTime) {
    tempoRegeneracao += deltaTime;

    if(tempoRegeneracao >= intervaloRegeneracao) {
        if (vida < vidaMaxima) {
            vida++;
        }
        tempoRegeneracao = 0.0f;
    }
}

void Base::desenhar() const {
    al_draw_filled_ellipse(posicao.x + largura / 2.0f, posicao.y + altura / 2.0f, largura * 0.62f, altura * 0.42f, al_map_rgb(125, 82, 51));
    al_draw_filled_ellipse(posicao.x + largura / 2.0f, posicao.y + altura / 2.0f + 5.0f, largura * 0.48f, altura * 0.26f, al_map_rgb(94, 64, 34));
    al_draw_filled_triangle(posicao.x + 24.0f, posicao.y + 34.0f, posicao.x + largura / 2.0f, posicao.y + 12.0f, posicao.x + largura - 24.0f, posicao.y + 34.0f, al_map_rgb(160, 120, 70));

    int ovosVisiveis = (vida * 12) / vidaMaxima;
    for (int i = 0; i < 12; ++i) {
        float offsetX = posicao.x + 18.0f + i * 14.0f;
        float offsetY = posicao.y + 30.0f - (i % 2 == 0 ? 2.0f : 0.0f);
        ALLEGRO_COLOR corOvo = (i < ovosVisiveis) ? al_map_rgb(250, 245, 230) : al_map_rgb(140, 120, 100);
        al_draw_filled_ellipse(offsetX, offsetY, 5.0f, 7.0f, corOvo);
        al_draw_ellipse(offsetX, offsetY, 5.0f, 7.0f, al_map_rgb(90, 70, 50), 1.0f);
    }

    float barraLargura = largura * (static_cast<float>(vida) / vidaMaxima);
    al_draw_filled_rectangle(posicao.x, posicao.y - 10, posicao.x + barraLargura, posicao.y - 5, al_map_rgb(255, 220, 180));
}

void Base::receberDano(int dano) {
    vida -= dano;
    if (vida < 0) {
        vida = 0;
    }
}

bool Base::estaDestruida() const {
    return vida <= 0;
}

bool Base::contemPonto(const Vetor2D& ponto) const {
    return ponto.x >= posicao.x && ponto.x <= posicao.x + largura &&
           ponto.y >= posicao.y && ponto.y <= posicao.y + altura;
}

Vetor2D Base::getPosicao() const {
    return posicao + Vetor2D(largura / 2.0f, altura / 2.0f); // Retorna o centro da base
}

int Base::getVida() const {
    return vida;
}

int Base::getVidaMaxima() const {
    return vidaMaxima;
}

