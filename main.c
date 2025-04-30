#include "raylib.h"

int main(void)
{
    const int tamLargura = 800;
    const int tamAltura = 450;
    const int groundAltura = 50;

    InitWindow(tamLargura, tamAltura, "The plumber Duo");
    
    Texture2D p1 = LoadTexture("assets/p1-removebg-preview.png");
    Texture2D p2 = LoadTexture("assets/p2.png");
    
    Vector2 player1 = { 200, tamAltura - groundAltura - 40 };
    Vector2 player2 = { 600, tamAltura - groundAltura - 40 };

    const float speed = 4.0f;
    const int size = 40;
    
    float velocidadeY1 = 0.0f;
    const float gravidade = 0.3f;
    const float jump = 8.0f;
    bool noChao1 = false;

    float velocidadeY2 = 0.0f;
    const float gravidade2 = 0.3f;
    const float jump2 = 8.0f;
    bool noChao2 = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        velocidadeY1 += gravidade;
        player1.y += velocidadeY1;

        velocidadeY2 += gravidade2;
        player2.y += velocidadeY2;

        if (player1.y >= tamAltura - groundAltura - size) {
            player1.y = tamAltura - groundAltura - size;
            velocidadeY1 = 0;
            noChao1 = true;
        } else {
            noChao1 = false;
        }

        if (IsKeyDown(KEY_RIGHT)){
            player1.x += speed;
        }
            
        if (IsKeyDown(KEY_LEFT)){ 
            player1.x -= speed;
        }
        if (IsKeyDown(KEY_DOWN)){
            player1.y +=speed;
        }

        if (IsKeyPressed(KEY_UP) && noChao1) {
            velocidadeY1 = -jump; 
        }

        if (player2.y >= tamAltura - groundAltura - size) {
            player2.y = tamAltura - groundAltura - size;
            velocidadeY2 = 0;
            noChao2 = true;
        } else {
            noChao2 = false;
        }

        if(IsKeyDown(KEY_W)&& noChao2){
            velocidadeY2 = -jump2;
        }
        if(IsKeyDown(KEY_A)){
            player2.x -= speed;
        }
        if(IsKeyDown(KEY_S)){
            player2.y +=speed;
        }
        if (IsKeyDown(KEY_D)) {
            player2.x += speed;
        }
        

        if (player1.x < 0){
            player1.x = 0;
        }
            
        if (player1.x > tamLargura - size){
            player1.x = tamLargura - size;
        }

        if (player2.x < 0){
            player2.x = 0;
        }

        if (player2.x > tamLargura - size) {
            player2.x = tamLargura - size;
        }

        if (player2.y < 0) {
            player2.y = 0;
        }
        if (player2.y > tamAltura - groundAltura - size){ 
            player2.y = tamAltura - groundAltura - size;
        }
        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawRectangle(0, tamAltura - groundAltura, tamLargura, groundAltura, BROWN);

        DrawRectangle(300, 250, 50, 50, GOLD);
        DrawRectangle(350, 250, 50, 50, GOLD);
        DrawRectangle(400, 250, 50, 50, GOLD);

        DrawCircle(150, 100, 20, WHITE);
        DrawCircle(170, 90, 25, WHITE);
        DrawCircle(190, 100, 20, WHITE);

        DrawCircle(500, 80, 20, WHITE);
        DrawCircle(520, 70, 25, WHITE);
        DrawCircle(540, 80, 20, WHITE);

        DrawText("Player 1: Setas | Player 2: WASD", 10, 10, 20, DARKBLUE);

        DrawTexturePro(
            p1,
            (Rectangle){0, 0, p1.width, p1.height},
            (Rectangle){player1.x, player1.y, 50, 50},
            (Vector2){0, 0},
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
