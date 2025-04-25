#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Dois Jogadores - Raylib");

    Vector2 player1 = { 200, 225 };
    Vector2 player2 = { 600, 225 };
    
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

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Player 1: Setas | Player 2: WASD", 10, 10, 20, DARKGRAY);

        DrawRectangleV(player1, (Vector2){size, size}, BLUE);
        DrawRectangleV(player2, (Vector2){size, size}, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
