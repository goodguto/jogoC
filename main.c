#include "raylib.h"

int main(void)
{
    const int tamLargura = 800;
    const int tamAltura = 450;
    const int groundAltura = 22;

    InitWindow(tamLargura, tamAltura, "The plumber Duo");

    Texture2D p1 = LoadTexture("assets/p1-removebg-preview.png");
    Texture2D p2 = LoadTexture("assets/p2.png");
    Texture2D ground = LoadTexture("assets/ground1.png");
    Texture2D floatblock = LoadTexture("assets/floatblock.png"); // NOVO BLOCO

    Vector2 player1 = { 200, tamAltura - groundAltura - 40 };
    Vector2 player2 = { 600, tamAltura - groundAltura - 40 };

    const float speed = 4.0f;
    const int size = 40;

    float velocidadeY1 = 0.0f;
    const float gravidade = 0.3f;
    const float jump = 9.0f;
    bool noChao1 = false;

    float velocidadeY2 = 0.0f;
    const float gravidade2 = 0.3f;
    const float jump2 = 9.0f;
    bool noChao2 = false;

    int escolha = 0;

    // Blocos flutuantes (como chão)
    Rectangle blocos[3] = {
        { 300, 320, 10, 50 },
        { 350, 320, 10, 50 },
        { 400, 320, 10, 50 }
    };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (escolha == 0)
        {
            DrawText("THE PLUMBER DUO", 250, 80, 30, DARKBLUE);
            DrawText("Escolha seu jogador:", 270, 150, 20, DARKGRAY);
            DrawText("Pressione 1 - Jogador 1 (Setas)", 230, 200, 20, DARKBLUE);
            DrawText("Pressione 2 - Jogador 2 (WASD)", 230, 240, 20, DARKBLUE);

            if (IsKeyPressed(KEY_ONE)){
                escolha = 1;
            }
            if (IsKeyPressed(KEY_TWO)){
                escolha = 2;
            }
        }
        else
        {
            velocidadeY1 += gravidade;
            player1.y += velocidadeY1;

            velocidadeY2 += gravidade2;
            player2.y += velocidadeY2;

            noChao1 = false;
            noChao2 = false;

            // Colisão com o chão
            if (player1.y >= tamAltura - groundAltura - size) {
                player1.y = tamAltura - groundAltura - size;
                velocidadeY1 = 0;
                noChao1 = true;
            }
            if (player2.y >= tamAltura - groundAltura - size) {
                player2.y = tamAltura - groundAltura - size;
                velocidadeY2 = 0;
                noChao2 = true;
            }

            // Colisão com blocos flutuantes
            Rectangle p1Rect = { player1.x, player1.y, size, size };
            Rectangle p2Rect = { player2.x, player2.y, size, size };

            for (int i = 0; i < 3; i++) {
                if (CheckCollisionRecs(p1Rect, blocos[i]) && velocidadeY1 >= 0 &&
                    player1.y + size - velocidadeY1 <= blocos[i].y) {
                    player1.y = blocos[i].y - size;
                    velocidadeY1 = 0;
                    noChao1 = true;
                }
                if (CheckCollisionRecs(p2Rect, blocos[i]) && velocidadeY2 >= 0 &&
                    player2.y + size - velocidadeY2 <= blocos[i].y) {
                    player2.y = blocos[i].y - size;
                    velocidadeY2 = 0;
                    noChao2 = true;
                }
            }

            if (escolha == 1) {
                if (IsKeyDown(KEY_RIGHT)) player1.x += speed;
                if (IsKeyDown(KEY_LEFT)) player1.x -= speed;
                if (IsKeyDown(KEY_DOWN)) player1.y += speed;
                if (IsKeyPressed(KEY_UP) && noChao1) velocidadeY1 = -jump;
            }

            if (escolha == 2) {
                if (IsKeyDown(KEY_D)) player2.x += speed;
                if (IsKeyDown(KEY_A)) player2.x -= speed;
                if (IsKeyDown(KEY_S)) player2.y += speed;
                if (IsKeyDown(KEY_W) && noChao2) velocidadeY2 = -jump2;
            }

            // Limites da tela
            if (player1.x < 0) player1.x = 0;
            if (player1.x > tamLargura - size) player1.x = tamLargura - size;
            if (player2.x < 0) player2.x = 0;
            if (player2.x > tamLargura - size) player2.x = tamLargura - size;
            if (player2.y < 0) player2.y = 0;
            if (player2.y > tamAltura - groundAltura - size) player2.y = tamAltura - groundAltura - size;

            // Chão com imagem
            for (int x = 0; x < tamLargura; x += ground.width) {
                DrawTexture(ground, x, tamAltura - ground.height, WHITE);
            }

            // Blocos flutuantes com imagem
            for (int i = 0; i < 3; i++) {
                DrawTextureEx(floatblock, (Vector2){blocos[i].x, blocos[i].y}, 0.0f, 50.0f / floatblock.width, WHITE);
            }

            // Nuvens
            DrawCircle(150, 100, 20, WHITE);
            DrawCircle(170, 90, 25, WHITE);
            DrawCircle(190, 100, 20, WHITE);
            DrawCircle(500, 80, 20, WHITE);
            DrawCircle(520, 70, 25, WHITE);
            DrawCircle(540, 80, 20, WHITE);

            DrawText("Player 1: Setas | Player 2: WASD", 10, 10, 20, DARKBLUE);

            if (escolha == 1) {
                DrawTexturePro(p1, (Rectangle){0, 0, p1.width, p1.height}, (Rectangle){player1.x, player1.y, 50, 50}, (Vector2){0, 0}, 0.0f, WHITE);
            }

            if (escolha == 2) {
                DrawTexturePro(p2, (Rectangle){0, 0, p2.width, p2.height}, (Rectangle){player2.x, player2.y, 50, 50}, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }

        EndDrawing();
    }

    UnloadTexture(p1);
    UnloadTexture(p2);
    UnloadTexture(ground);
    UnloadTexture(floatblock);

    CloseWindow();
    return 0;
}
