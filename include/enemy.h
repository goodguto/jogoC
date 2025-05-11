#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_FRAMES 20

typedef struct {
    Vector2 pos;
    Texture2D frames[MAX_FRAMES];
    int frameCount;
    int frameAtual;
    float tempoFrame;
    float intervalo;
} Enemy;

Enemy CarregarInimigo(const char *jsonPath);
void AtualizarInimigo(Enemy *e);
void DesenharInimigo(Enemy e);
void LiberarInimigo(Enemy *e);

#endif
