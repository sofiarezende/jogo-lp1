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
    // Desenhar a base como um retângulo
    al_draw_filled_rectangle(posicao.x, posicao.y, posicao.x + largura, posicao.y + altura, al_map_rgb(0, 255, 0));

    // Desenhar a barra de vida acima da base
    float barraLargura = largura * (static_cast<float>(vida) / vidaMaxima);
    al_draw_filled_rectangle(posicao.x, posicao.y - 10, posicao.x + barraLargura, posicao.y - 5, al_map_rgb(255, 0, 0));
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
    return posicao;
}

