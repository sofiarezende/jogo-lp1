#include "../include/heroi.h"
#include "../include/projetil.h"
#include "../include/vetor2d.h"

#include <allegro5/allegro.h>           
#include <allegro5/allegro_image.h>     
#include <allegro5/allegro_primitives.h>

#include <iostream>

Heroi::Heroi(const Vetor2D& posicaoInicial, int vidaMaxima, int municaoInicial, float alcanceMaximoProjetil)
    : posicao(posicaoInicial), vida(vidaMaxima), vidaMaxima(vidaMaxima), angulo(0.0f) {
    imagemTartaruga = al_load_bitmap("../assets/tartaruga.png");
    if (!imagemTartaruga) {
        fprintf(stderr, "Falha ao carregar a imagem da tartaruga.\n");
    }
}

void Heroi::desenhar() const {
    if (imagemTartaruga) {
        // Dimensões exatas calculadas por frame
        float frameLargura = 96.0f;
        float frameAltura = 48.0f;

        // Índices corretos da folha de sprite (0-based)
        // Linha 4 (quarta linha de cima para baixo) -> Índice 3
        // Coluna 1 (primeira coluna da esquerda)     -> Índice 0
        int indiceColuna = 0; 
        int indiceLinha = 3;  

        float corteX = indiceColuna * frameLargura;
        float corteY = indiceLinha * frameAltura;

        // Cria o recorte exato da tartaruga escolhida
        ALLEGRO_BITMAP* subTartaruga = al_create_sub_bitmap(imagemTartaruga, corteX, corteY, frameLargura, frameAltura);

        if (subTartaruga) {
            // Desenha o recorte rotacionando pelo centro dele
            al_draw_scaled_rotated_bitmap(subTartaruga, frameLargura / 2.0f, frameAltura / 2.0f, posicao.x, posicao.y, 1.0f, 1.0f, angulo, 0);

            // Destrói o sub-bitmap temporário para limpar a memória
            al_destroy_bitmap(subTartaruga);
        }
    } else {
        // Fallback caso a imagem dê erro
        al_draw_filled_circle(posicao.x, posicao.y, 15, al_map_rgb(0, 255, 0));
    }
}

Heroi::~Heroi() {
    if (imagemTartaruga) {
        al_destroy_bitmap(imagemTartaruga);
    }
}

void Heroi::definirDestino(const Vetor2D& destino) {
    this->destino = destino;
}

void Heroi::atualizar(float deltaTime) {
    Vetor2D direcao = destino - posicao;
    float distancia = direcao.magnitude();

    if (distancia > 1.0f) {
        direcao.normalizar();
        posicao = posicao + direcao * 200.0f * deltaTime; // Velocidade de 200 unidades por segundo
        angulo = std::atan2(direcao.y, direcao.x);
    }
}

void Heroi::receberDano(int dano) {
    vida -= dano;
    if (vida < 0) {
        vida = 0;
    }
}

bool Heroi::estaVivo() const {
    return vida > 0;
}

void Heroi::atirar(Vetor2D& posicaoMouse, std::list<Projetil>& listaProjetis) {
    if (municao > 0) {
        Vetor2D direcao = posicaoMouse - posicao;
        direcao.normalizar();
        Projetil novoProjetil(posicao, direcao, 500.0f, alcanceMaximoProjetil, Dono::HEROI);
        listaProjetis.push_back(novoProjetil);
        municao--;
    }
}

void Heroi::ganharMunicao(int quantidade) {
    municao += quantidade;
}

int Heroi::getMunicao() const {
    return municao;
}

Vetor2D Heroi::getPosicao() const {
    return posicao;
}

int Heroi::getVida() const {
    return vida;
}
