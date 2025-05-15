#include <stdio.h>
#include "raylib.h"
#include "game.h"

void SalvarRanking(double tempoFinal);
void MostrarRanking();

int main(void) {
    InitWindow(LARGURA, ALTURA, "The Plumber Duo");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    Player player1, player2;
    Texture2D ground, bloco, cano;
    CarregarTexturas(&player1, &player2, &ground, &bloco, &cano);

    InitPlayers(&player1, &player2);

    #define MAX_BLOCOS 1000
    Rectangle blocos[MAX_BLOCOS];
    int numBlocos = 0;

    float alturaGerada = ALTURA - GROUND_ALTURA - PLAYER_SIZE - 100;
    float alturaFinal = alturaGerada - 100;
    bool baseFinalCriada = false;

    float alturaUltimoAndar = player1.pos.y;
    float alturaMaxima = player1.pos.y;

    GerarBlocosNivel(alturaUltimoAndar - 100, blocos, &numBlocos);

    int escolha = 0;
    int menuIndex = 0;
    const int totalOpcoesMenu = 4;
    bool noMenu = true;

    double tempoInicio = GetTime();
    double tempoFinal = 0;
    bool venceu = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode2D(camera);

        if (escolha == 0) {
            const char *opcoesMenu[] = {
                "Jogar com Jogador 1 (WASD)",
                "Jogar com Jogador 2 (WASD)",
                "Ver Ranking",
                "Sair"
            };

            DrawText("THE PLUMBER DUO", 250, 80, 30, DARKBLUE);
            DrawText("Use ↑ ↓ e Enter para navegar", 220, 130, 20, DARKGRAY);

            if (IsKeyPressed(KEY_DOWN)) {
                menuIndex = (menuIndex + 1) % totalOpcoesMenu;
            }
            if (IsKeyPressed(KEY_UP)) {
                menuIndex = (menuIndex - 1 + totalOpcoesMenu) % totalOpcoesMenu;
            }

            for (int i = 0; i < totalOpcoesMenu; i++) {
                Color cor;
                if (i == menuIndex) {
                    cor = RED;
                } else {
                    cor = DARKBLUE;
                }
                DrawText(opcoesMenu[i], 230, 200 + i * 40, 20, cor);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (menuIndex == 0) {
                    escolha = 1;
                } else if (menuIndex == 1) {
                    escolha = 2;
                } else if (menuIndex == 2) {
                    ClearBackground(SKYBLUE);
                    MostrarRanking();
                    EndMode2D();
                    EndDrawing();

                    while (!WindowShouldClose()) {
                        if (IsKeyPressed(KEY_ESCAPE)) {
                            break;
                        }
                        BeginDrawing();
                        ClearBackground(SKYBLUE);
                        BeginMode2D(camera);
                        MostrarRanking();
                        EndMode2D();
                        EndDrawing();
                    }
                } else if (menuIndex == 3) {
                    break;
                }
            }
        } else {
            UpdateFisica(&player1, blocos, numBlocos);
            UpdateFisica(&player2, blocos, numBlocos);
            DesenharCenario(ground, blocos, numBlocos, bloco, cano);

            if (escolha == 1) {
                AtualizarCamera(&camera, player1.pos);
            } else {
                AtualizarCamera(&camera, player2.pos);
            }

            if (venceu) {
                char texto[50];
                sprintf(texto, "Parabéns! Tempo: %.2f segundos", tempoFinal);
                DrawText(texto, 200, 100, 30, RED);
            }

            Vector2 posJogador;
            if (escolha == 1) {
                posJogador = player1.pos;
            } else {
                posJogador = player2.pos;
            }

            if (posJogador.y < alturaUltimoAndar - 100 && !baseFinalCriada) {
                alturaUltimoAndar -= 100;

                if (alturaUltimoAndar > -2500) {
                    GerarBlocosNivel(alturaUltimoAndar, blocos, &numBlocos);
                } else {
                    float yBase = alturaUltimoAndar;
                    float xBase[] = {280, 330, 380, 430};
                    double tempoFinal = 0;
                    bool venceu = false;

                    for (int i = 0; i < 4; i++) {
                        blocos[numBlocos++] = (Rectangle){ xBase[i], yBase, 50, 10 };
                    }

                    baseFinalCriada = true;
                }
            }

            if (baseFinalCriada && !venceu) {
                Rectangle jogador = { posJogador.x, posJogador.y, PLAYER_SIZE, PLAYER_SIZE };

                for (int i = 0; i < numBlocos; i++) {
                    if (blocos[i].y == alturaUltimoAndar && CheckCollisionRecs(jogador, blocos[i])) {
                        tempoFinal = GetTime() - tempoInicio;
                        venceu = true;
                        SalvarRanking(tempoFinal);
                        break;
                    }
                }
            }

            if (escolha == 1) {
                UpdateControles(&player1, 1);
            }
            if (escolha == 2) {
                UpdateControles(&player2, 2);
            }

            DesenharCenario(ground, blocos, 3, bloco, cano);
            if (escolha == 1) {
                DesenharJogador(player1);
            }
            if (escolha == 2) {
                DesenharJogador(player2);
            }
        }

        EndMode2D();
        DrawText("Player 1: Setas | Player 2: WASD", 10, 10, 20, DARKBLUE);
        EndDrawing();
    }

    LiberarTexturas(player1, player2, ground, bloco, cano);
    CloseWindow();
    return 0;
}

void SalvarRanking(double tempoFinal) {
    FILE *arquivo = fopen("ranking.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%.2f\n", tempoFinal);
        fclose(arquivo);
    }
}

void MostrarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) {
        DrawText("Sem ranking disponível", 250, 300, 20, DARKGRAY);
        return;
    }

    char linha[100];
    int y = 300;
    int pos = 1;
    while (fgets(linha, sizeof(linha), arquivo) && pos <= 5) {
        char texto[120];
        sprintf(texto, "%dº lugar: %s", pos, linha);
        DrawText(texto, 250, y, 20, DARKBLUE);
        y += 30;
        pos++;
    }

    fclose(arquivo);
}
