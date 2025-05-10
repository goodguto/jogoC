#include "game.h"

void InitPlayers(Player *p1, Player *p2) {
    p1->pos = (Vector2){ 200, ALTURA - GROUND_ALTURA - PLAYER_SIZE };
    p1->velocidadeY = 0.0f;
    p1->noChao = false;

    p2->pos = (Vector2){ 600, ALTURA - GROUND_ALTURA - PLAYER_SIZE };
    p2->velocidadeY = 0.0f;
    p2->noChao = false;
}

void UpdateFisica(Player *p, Rectangle *blocos, int numBlocos) {
    const float gravidade = 0.3f;
    p->velocidadeY += gravidade;
    p->pos.y += p->velocidadeY;
    p->noChao = false;

    // Chão
    if (p->pos.y >= ALTURA - GROUND_ALTURA - PLAYER_SIZE) {
        p->pos.y = ALTURA - GROUND_ALTURA - PLAYER_SIZE;
        p->velocidadeY = 0;
        p->noChao = true;
    }

    // Colisão blocos
    Rectangle r = { p->pos.x, p->pos.y, PLAYER_SIZE, PLAYER_SIZE };
    for (int i = 0; i < numBlocos; i++) {
        if (CheckCollisionRecs(r, blocos[i]) && p->velocidadeY >= 0 &&
            p->pos.y + PLAYER_SIZE - p->velocidadeY <= blocos[i].y) {
            p->pos.y = blocos[i].y - PLAYER_SIZE;
            p->velocidadeY = 0;
            p->noChao = true;
        }
    }
}

void UpdateControles(Player *p, int tipo) {
    const float speed = 4.0f;
    const float jump = 9.0f;

    if (tipo == 1) {
        if (IsKeyDown(KEY_RIGHT)) p->pos.x += speed;
        if (IsKeyDown(KEY_LEFT)) p->pos.x -= speed;
        if (IsKeyPressed(KEY_UP) && p->noChao) p->velocidadeY = -jump;
    } else if (tipo == 2) {
        if (IsKeyDown(KEY_D)) p->pos.x += speed;
        if (IsKeyDown(KEY_A)) p->pos.x -= speed;
        if (IsKeyPressed(KEY_W) && p->noChao) p->velocidadeY = -jump;
    }

    // Limites tela
    if (p->pos.x < 0) p->pos.x = 0;
    if (p->pos.x > LARGURA - PLAYER_SIZE) p->pos.x = LARGURA - PLAYER_SIZE;
}

void DesenharJogador(Player p) {
    DrawTexturePro(p.textura, (Rectangle){0, 0, p.textura.width, p.textura.height},
        (Rectangle){p.pos.x, p.pos.y, 50, 50}, (Vector2){0, 0}, 0.0f, WHITE);
}

void DesenharCenario(Texture2D ground, Rectangle *blocos, int numBlocos, Texture2D blocoImg) {
    for (int x = 0; x < LARGURA; x += ground.width) {
        DrawTexture(ground, x, ALTURA - ground.height, WHITE);
    }

    for (int i = 0; i < numBlocos; i++) {
        DrawTextureEx(blocoImg, (Vector2){blocos[i].x, blocos[i].y}, 0.0f,
                      50.0f / blocoImg.width, WHITE);
    }

    // Nuvens
    DrawCircle(150, 100, 20, WHITE);
    DrawCircle(170, 90, 25, WHITE);
    DrawCircle(190, 100, 20, WHITE);
    DrawCircle(500, 80, 20, WHITE);
    DrawCircle(520, 70, 25, WHITE);
    DrawCircle(540, 80, 20, WHITE);
}

void CarregarTexturas(Player *p1, Player *p2, Texture2D *ground, Texture2D *bloco) {
    p1->textura = LoadTexture("assets/player_colision/p1-removebg-preview.png");
    p2->textura = LoadTexture("assets/player_colision/p2.png");
    *ground = LoadTexture("assets/map/ground1.png");
    *bloco = LoadTexture("assets/map/floatblock.png");
}

void LiberarTexturas(Player p1, Player p2, Texture2D ground, Texture2D bloco) {
    UnloadTexture(p1.textura);
    UnloadTexture(p2.textura);
    UnloadTexture(ground);
    UnloadTexture(bloco);
}
