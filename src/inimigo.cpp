#include "../include/inimigo.h"
#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Inimigo::Inimigo(const Vetor2D& posicaoInicial, int vidaMaxima, float alcanceMaximoProjetil)
    : posicao(posicaoInicial), vida(vidaMaxima), vidaMaxima(vidaMaxima) {}

void Inimigo::atualizar(float deltaTime, const Vetor2D& alvo, std::list<Projetil>& listaProjetis) {
    cronometroTiro += deltaTime;

    if (cronometroTiro >= intervaloEntreTiros) {
        Vetor2D direcao = alvo - posicao;
        float distanciaAlvo = direcao.magnitude();

        if (distanciaAlvo <= 100.0f) { // Alcance máximo do projétil
            direcao.normalizar();
            Projetil novoProjetil(posicao, direcao, 500.0f, 100.0f, Dono::INIMIGO);
            listaProjetis.push_back(novoProjetil);
            cronometroTiro = 0.0f; // Reinicia o cronômetro de tiro
        }
    }
}

void Inimigo::desenhar() const {
    al_draw_filled_rectangle(posicao.x - 15, posicao.y - 15, posicao.x + 15, posicao.y + 15, al_map_rgb(255, 0, 0));
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
    return 10; // Retorna a quantidade de munição largada pelo inimigo
}

