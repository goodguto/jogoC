#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int groundHeight = 50;

    InitWindow(screenWidth, screenHeight, "Dois Jogadores - Estilo Mario");
    Texture2D p1 = LoadTexture("assets/p1-removebg-preview.png");
    Texture2D p2 = LoadTexture("assets/p2.png");
    Vector2 player1 = { 200, screenHeight - groundHeight - 40 };
    Vector2 player2 = { 600, screenHeight - groundHeight - 40 };
    
    const float speed = 4.0f;
    const int size = 40;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        
        if (IsKeyDown(KEY_RIGHT)) player1.x += speed;
        if (IsKeyDown(KEY_LEFT))  player1.x -= speed;
        if (IsKeyDown(KEY_UP))    player1.y -= speed;
        if (IsKeyDown(KEY_DOWN))  player1.y += speed;

        if (IsKeyDown(KEY_D)) player2.x += speed;
        if (IsKeyDown(KEY_A)) player2.x -= speed;
        if (IsKeyDown(KEY_W)) player2.y -= speed;
        if (IsKeyDown(KEY_S)) player2.y += speed;

        if (IsGamepadAvailable(0))
        {
            float axisX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float axisY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            player1.x += axisX * speed;
            player1.y += axisY * speed;
        }

        //limete para nao sair da tela
        if (player1.x < 0) player1.x = 0;
        if (player1.x > screenWidth - size) player1.x = screenWidth - size;
        if (player1.y < 0) player1.y = 0;
        if (player1.y > screenHeight - groundHeight - size) player1.y = screenHeight - groundHeight - size;

        // limete para nao sair da tela
        if (player2.x < 0) player2.x = 0;
        if (player2.x > screenWidth - size) player2.x = screenWidth - size;
        if (player2.y < 0) player2.y = 0;
        if (player2.y > screenHeight - groundHeight - size) player2.y = screenHeight - groundHeight - size;

        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawRectangle(0, screenHeight - groundHeight, screenWidth, groundHeight, BROWN);

        DrawRectangle(300, 250, 50, 50, GOLD);
        DrawRectangle(350, 250, 50, 50, GOLD);
        DrawRectangle(400, 250, 50, 50, GOLD);

        DrawCircle(150, 100, 20, WHITE);
        DrawCircle(170, 90, 25, WHITE);
        DrawCircle(190, 100, 20, WHITE);

        DrawCircle(500, 80, 20, WHITE);
        DrawCircle(520, 70, 25, WHITE);
        DrawCircle(540, 80, 20, WHITE);

        DrawText("Player 1: Setas ou Controle 0 | Player 2: WASD ou Controle 1", 10, 10, 20, DARKBLUE);

        DrawTexturePro(
            p1,// textura do p1
            (Rectangle){0, 0, p1.width, p1.height},
            (Rectangle){player1.x, player1.y, 50, 50},//os 50,50 são os sizes(alterar tamanho)
            (Vector2){0, 0},//serve para rotacionar
            0.0f,
            WHITE                                           
        );
        
        DrawTexturePro(
            p2, 
            (Rectangle){0, 0, p2.width, p2.height},
            (Rectangle){player2.x, player2.y, 50, 50},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
