#pragma once
#include "heroi.h"
#include "base.h"
#include "inimigo.h"
#include "projetil.h"
#include "municao.h"

#include <list>
#include <vector>

struct ALLEGRO_BITMAP;
struct ALLEGRO_SAMPLE;




enum class EstadoJogo { JOGANDO, VITORIA, DERROTA };

class Jogo {
public:
    Jogo(int larguraMapa, int alturaMapa, ALLEGRO_SAMPLE* somTiro = nullptr, ALLEGRO_SAMPLE* somDanoArea = nullptr);
    ~Jogo();

    void processarClique(Vetor2D posicaoMouse);
    void processarTiro(Vetor2D posicaoMouse);
    void processarMovimentoTeclado(const Vetor2D& direcao, float dt);
    void atualizar(float dt);
    void reiniciar();
    void desenhar(const std::vector<ALLEGRO_BITMAP*>& spritesHeroi,
                  const std::vector<ALLEGRO_BITMAP*>& spritesItens,
                  ALLEGRO_BITMAP* spriteGaivota) const;

    EstadoJogo getEstado() const;
    float getTempoSobrevivido() const;
    float getTempoParaVencer() const;
    int getVidaHeroi() const;
    int getVidaHeroiMaxima() const;
    int getMunicaoHeroi() const;
    int getVidaBase() const;
    int getVidaBaseMaxima() const;


    

private:
    int larguraMapa, alturaMapa;

    Heroi heroi;
    Base base;
    std::list<Inimigo> inimigos;
    std::list<Projetil> projeteis;
    std::list<Municao> municoesNoChao;

    float cronometroSpawn = 0.0f;
    float intervaloSpawnAtual = 3.0f;

    float cronometroZonaSegura = 0.0f;
    float intervaloCuraZonaSegura = 1.5f;

    float tempoSobrevivido = 0.0f;
    float tempoParaVencer = 600.0f; // 10 minutos

    EstadoJogo estado = EstadoJogo::JOGANDO;

    ALLEGRO_SAMPLE* somTiro = nullptr;
    ALLEGRO_SAMPLE* somDanoArea = nullptr;

    
    void atualizarInimigos(float dt);
    void atualizarProjeteis(float dt);
    void atualizarMunicoes(float dt);
    void atualizarZonaSegura(float dt);
    void verificarColisoes();
    void verificarSpawnDeInimigos(float dt);
    void verificarCondicoesDeFim(float dt);
    void tocarSomTiro() const;
    void tocarSomDanoArea() const;
    
};