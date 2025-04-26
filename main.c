#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int groundHeight = 50;

    InitWindow(screenWidth, screenHeight, "Dois Jogadores - Estilo Mario");

    Vector2 player1 = { 200, screenHeight - groundHeight - 40 }; // Começa no chão
    Vector2 player2 = { 600, screenHeight - groundHeight - 40 };
    
    const float speed = 4.0f;
    const int size = 40;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Movimento do Player 1 (Teclado)
        if (IsKeyDown(KEY_RIGHT)) player1.x += speed;
        if (IsKeyDown(KEY_LEFT))  player1.x -= speed;
        if (IsKeyDown(KEY_UP))    player1.y -= speed;
        if (IsKeyDown(KEY_DOWN))  player1.y += speed;

        // Movimento do Player 2 (Teclado)
        if (IsKeyDown(KEY_D)) player2.x += speed;
        if (IsKeyDown(KEY_A)) player2.x -= speed;
        if (IsKeyDown(KEY_W)) player2.y -= speed;
        if (IsKeyDown(KEY_S)) player2.y += speed;

        // Movimento com Controle para Player 1
        if (IsGamepadAvailable(0))
        {
            float axisX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float axisY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            player1.x += axisX * speed;
            player1.y += axisY * speed;
        }

        // Movimento com Controle para Player 2
        if (IsGamepadAvailable(1))
        {
            float axisX = GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_X);
            float axisY = GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_Y);

            player2.x += axisX * speed;
            player2.y += axisY * speed;
        }

        // --- LIMITES DA TELA para Player 1 ---
        if (player1.x < 0) player1.x = 0;
        if (player1.x > screenWidth - size) player1.x = screenWidth - size;
        if (player1.y < 0) player1.y = 0;
        if (player1.y > screenHeight - groundHeight - size) player1.y = screenHeight - groundHeight - size;

        // --- LIMITES DA TELA para Player 2 ---
        if (player2.x < 0) player2.x = 0;
        if (player2.x > screenWidth - size) player2.x = screenWidth - size;
        if (player2.y < 0) player2.y = 0;
        if (player2.y > screenHeight - groundHeight - size) player2.y = screenHeight - groundHeight - size;

        BeginDrawing();
        ClearBackground(SKYBLUE); // Céu azul

        // --- CHÃO ---
        DrawRectangle(0, screenHeight - groundHeight, screenWidth, groundHeight, BROWN);

        // --- BLOCOS NO AR ---
        DrawRectangle(300, 250, 50, 50, GOLD);
        DrawRectangle(350, 250, 50, 50, GOLD);
        DrawRectangle(400, 250, 50, 50, GOLD);

        // --- NUVENS ---
        DrawCircle(150, 100, 20, WHITE);
        DrawCircle(170, 90, 25, WHITE);
        DrawCircle(190, 100, 20, WHITE);

        DrawCircle(500, 80, 20, WHITE);
        DrawCircle(520, 70, 25, WHITE);
        DrawCircle(540, 80, 20, WHITE);

        // --- TEXTO ---
        DrawText("Player 1: Setas ou Controle 0 | Player 2: WASD ou Controle 1", 10, 10, 20, DARKBLUE);

        // --- JOGADORES ---
        DrawRectangleV(player1, (Vector2){size, size}, BLUE);
        DrawRectangleV(player2, (Vector2){size, size}, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
