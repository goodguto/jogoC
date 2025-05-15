#include "game.h"

void InitPlayers(Player *p1, Player *p2) {
    p1->pos = (Vector2){ 200, ALTURA - GROUND_ALTURA - PLAYER_SIZE };
    p1->velocidadeY = 0.0f;
    p1->noChao = false;

    p2->pos = (Vector2){ 200, ALTURA - GROUND_ALTURA - PLAYER_SIZE };
    p2->velocidadeY = 0.0f;
    p2->noChao = false;
}
void AtualizarCamera(Camera2D *camera, Vector2 playerPos)
{
    Vector2 destino = { playerPos.x -350, playerPos.y -350}; // Centralizo
    camera->target.x += (destino.x - camera->target.x) * 0.1f; // deixa a movimentação da camera mais suave
    camera->target.y += (destino.y - camera->target.y) * 0.1f;
}
void UpdateFisica(Player *p, Rectangle *blocos, int numBlocos) {
    const float gravidade = 0.1f;
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
void GerarBlocosNivel(float yNivel, Rectangle *blocos, int *numBlocos) {
    if (*numBlocos + 5 >= MAX_BLOCOS) return;

    // Define posições fixas horizontais
    float posicoesX[] = {100, 250, 400, 550, 700};
    int numPosicoes = sizeof(posicoesX) / sizeof(float);

    // Escolhe quantos blocos por andar (ex: 2 ou 3 blocos)
    int chance = GetRandomValue(0, 100);
    int quantos;

    if (chance < 30) quantos = 1;         // 30% de chance de gerar só 1 bloco (difícil)
    else if (chance < 70) quantos = 2;    // 40% de chance de gerar 2 blocos (médio)
    else quantos = 3;                     // 30% de chance de gerar 3 blocos (fácil)


    for (int i = 0; i < quantos; i++) {
        int indice = GetRandomValue(0, numPosicoes - 1);
        float x = posicoesX[indice];

        // Verifica se já tem bloco na mesma posição (para evitar sobreposição)
        bool repetido = false;
        for (int j = 0; j < *numBlocos; j++) {
            if (blocos[j].x == x && blocos[j].y == yNivel) {
                repetido = true;
                break;
            }
        }

        if (!repetido) {
            blocos[*numBlocos] = (Rectangle){ x, yNivel, 50, 10 };
            (*numBlocos)++;
        }
    }
}


void UpdateControles(Player *p, int tipo) {
    const float speed = 4.0f;
    const float jump = 11.0f;

    if (tipo == 1) {
        if (IsKeyDown(KEY_D)) p->pos.x += speed;
        if (IsKeyDown(KEY_A)) p->pos.x -= speed;
        if (IsKeyPressed(KEY_W) && p->noChao) p->velocidadeY = -jump;
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

void DesenharCenario(Texture2D ground, Rectangle *blocos, int numBlocos, Texture2D blocoImg,Texture2D cano) {
    for (int x = 0; x < LARGURA; x += ground.width) {
        DrawTexture(ground, x, ALTURA - ground.height, WHITE);
    }

    for (int i = 0; i < numBlocos; i++) {
        DrawTextureEx(blocoImg, (Vector2){blocos[i].x, blocos[i].y}, 0.0f,
                      50.0f / blocoImg.width, WHITE);
    }
    DrawTextureEx(cano, (Vector2){600, ALTURA - GROUND_ALTURA - cano.height}, 0.0f, 1.0f, WHITE);
    Rectangle hitboxCano = {
        600, 
        ALTURA - GROUND_ALTURA - cano.height,
        cano.width,
        cano.height
    };
    
    // Nuvens
    DrawCircle(150, 100, 20, WHITE);
    DrawCircle(170, 90, 25, WHITE);
    DrawCircle(190, 100, 20, WHITE);
    DrawCircle(500, 80, 20, WHITE);
    DrawCircle(520, 70, 25, WHITE);
    DrawCircle(540, 80, 20, WHITE);
}


void CarregarTexturas(Player *p1, Player *p2, Texture2D *ground, Texture2D *bloco, Texture2D *cano) {
    p1->textura = LoadTexture("assets/player_colision/p1-removebg-preview.png");
    p2->textura = LoadTexture("assets/player_colision/p2.png");
    *ground = LoadTexture("assets/map/ground1.png");
    *bloco = LoadTexture("assets/map/floatblock.png");
    *cano = LoadTexture("assets/player_colision/cano.png");

}

void LiberarTexturas(Player p1, Player p2, Texture2D ground, Texture2D bloco, Texture2D cano) {
    UnloadTexture(p1.textura);
    UnloadTexture(p2.textura);
    UnloadTexture(ground);
    UnloadTexture(bloco);
    UnloadTexture(cano);

}

