#include "raylib.h"
#include "game.h"

int main(void) {
    InitWindow(LARGURA, ALTURA, "The Plumber Duo");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    Player player1, player2;
    Texture2D ground, bloco;
    CarregarTexturas(&player1, &player2, &ground, &bloco);
    InitPlayers(&player1, &player2);

    int escolha = 0;

    Rectangle blocos[] = {
        { 300, 320, 10, 50 },
        { 350, 320, 10, 50 },
        { 400, 320, 10, 50 }
    };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode2D(camera);

        if (escolha == 0) {
            DrawText("THE PLUMBER DUO", 250, 80, 30, DARKBLUE);
            DrawText("Escolha seu jogador:", 270, 150, 20, DARKGRAY);
            DrawText("Pressione 1 - Jogador 1 (Setas)", 230, 200, 20, DARKBLUE);
            DrawText("Pressione 2 - Jogador 2 (WASD)", 230, 240, 20, DARKBLUE);
            if (IsKeyPressed(KEY_ONE)) escolha = 1;
            if (IsKeyPressed(KEY_TWO)) escolha = 2;
        } else {
            UpdateFisica(&player1, blocos, 3);
            AtualizarCamera(&camera, player1.pos);
            UpdateFisica(&player2, blocos, 3);
            AtualizarCamera(&camera, player1.pos);

            if (escolha == 1) UpdateControles(&player1, 1);
            if (escolha == 2) UpdateControles(&player2, 2);

            DesenharCenario(ground, blocos, 3, bloco);
            if (escolha == 1) DesenharJogador(player1);
            if (escolha == 2) DesenharJogador(player2);
        }

        EndMode2D();
        DrawText("Player 1: Setas | Player 2: WASD", 10, 10, 20, DARKBLUE);
        EndDrawing();
    }

    LiberarTexturas(player1, player2, ground, bloco);
    CloseWindow();
    return 0;
}
