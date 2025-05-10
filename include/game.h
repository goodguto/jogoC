#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define LARGURA 800
#define ALTURA 450
#define GROUND_ALTURA 22
#define PLAYER_SIZE 40

typedef struct {
    Vector2 pos;
    float velocidadeY;
    bool noChao;
    Texture2D textura;
} Player;

void InitPlayers(Player *p1, Player *p2);
void AtualizarCamera(Camera2D *camera, Vector2 playerPos);
void UpdateFisica(Player *p, Rectangle *blocos, int numBlocos);
void UpdateControles(Player *p, int controleTipo);
void DesenharJogador(Player p);
void DesenharCenario(Texture2D ground, Rectangle *blocos, int numBlocos, Texture2D blocoImg);
void CarregarTexturas(Player *p1, Player *p2, Texture2D *ground, Texture2D *bloco);
void LiberarTexturas(Player p1, Player p2, Texture2D ground, Texture2D bloco);

#endif
