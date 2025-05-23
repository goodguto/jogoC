#include <stdio.h>
#include "raylib.h"
#include "game.h"

#define MAX_BLOCOS 1000
#define MAX_MOEDAS 100
#define MAX_INIMIGOS 50

typedef struct {
    Rectangle rect;
    bool coletada;
} Moeda;

typedef struct {
    Rectangle rect;
    bool ativo;
} Inimigo;

// Variáveis
Moeda moedas[MAX_MOEDAS];
int numMoedas = 0;

Inimigo inimigos[MAX_INIMIGOS];
int numInimigos = 0;

// Funções auxiliares
void SalvarRanking(int moedasColetadas, int tempo);
void MostrarRanking();
void GerarInimigo(float y);
void ResetarJogo(Player *player1, Player *player2, Rectangle *blocos, int *numBlocos, float *alturaUltimoAndar, bool *baseFinalCriada, double *tempoInicio);

// ===================================
// Função principal
// ===================================
int main(void) {
    InitWindow(LARGURA, ALTURA, "The Plumber Duo");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    Player player1, player2;
    Texture2D ground, bloco, cano;
    CarregarTexturas(&player1, &player2, &ground, &bloco, &cano);

    InitPlayers(&player1, &player2);

    Rectangle blocos[MAX_BLOCOS];
    int numBlocos = 0;

    float alturaUltimoAndar;
    bool baseFinalCriada;

    double tempoInicio;
    int tempoFinal;
    bool venceu;

    int escolha = 0;
    int menuIndex = 0;
    const int totalOpcoesMenu = 4;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (escolha == 0) {
            // MENU
            const char *opcoesMenu[] = {
                "Jogar com Jogador 1 (Setas)",
                "Jogar com Jogador 2 (WASD)",
                "Ver Ranking",
                "Sair"
            };

            int centroX = LARGURA / 2 - 200;

            DrawText("THE PLUMBER DUO", centroX + 50, 80, 30, DARKBLUE);
            DrawText("Use ↑ ↓ e Enter para navegar", centroX + 10, 130, 20, DARKGRAY);

            if (IsKeyPressed(KEY_DOWN)) menuIndex = (menuIndex + 1) % totalOpcoesMenu;
            if (IsKeyPressed(KEY_UP)) menuIndex = (menuIndex - 1 + totalOpcoesMenu) % totalOpcoesMenu;

            for (int i = 0; i < totalOpcoesMenu; i++) {
                Color cor = (i == menuIndex) ? RED : DARKBLUE;
                DrawText(opcoesMenu[i], centroX + 50, 200 + i * 40, 20, cor);
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (menuIndex == 0 || menuIndex == 1) {
                    escolha = (menuIndex == 0) ? 1 : 2;

                    ResetarJogo(&player1, &player2, blocos, &numBlocos, &alturaUltimoAndar, &baseFinalCriada, &tempoInicio);
                    venceu = false;
                } else if (menuIndex == 2) {
                    while (!WindowShouldClose()) {
                        BeginDrawing();
                        ClearBackground(SKYBLUE);
                        MostrarRanking();
                        DrawText("Pressione ESC para voltar", 10, 10, 20, DARKGRAY);
                        EndDrawing();

                        if (IsKeyPressed(KEY_ESCAPE)) break;
                    }
                } else if (menuIndex == 3) {
                    break;
                }
            }

        } else {
            BeginMode2D(camera);

            UpdateFisica(&player1, blocos, numBlocos);
            UpdateFisica(&player2, blocos, numBlocos);
            DesenharCenario(ground, blocos, numBlocos, bloco, cano);

            Vector2 posJogador = (escolha == 1) ? player1.pos : player2.pos;
            AtualizarCamera(&camera, posJogador);

            // Colisão com moedas
            for (int i = 0; i < numMoedas; i++) {
                if (!moedas[i].coletada) {
                    Rectangle jogador = { posJogador.x, posJogador.y, PLAYER_SIZE, PLAYER_SIZE };
                    if (CheckCollisionRecs(jogador, moedas[i].rect)) {
                        moedas[i].coletada = true;
                        if (escolha == 1) player1.moedas++;
                        else player2.moedas++;
                    }
                }
            }

            // Colisão com inimigos → MORREU → VOLTA PRO MENU
            Rectangle jogador = { posJogador.x, posJogador.y, PLAYER_SIZE, PLAYER_SIZE };
            for (int i = 0; i < numInimigos; i++) {
                if (inimigos[i].ativo && CheckCollisionRecs(jogador, inimigos[i].rect)) {
                    escolha = 0;
                    break;
                }
            }

            // Gerar andares
            if (posJogador.y < alturaUltimoAndar - 100 && !baseFinalCriada) {
                alturaUltimoAndar -= 100;

                if (alturaUltimoAndar > -2500) {
                    GerarBlocosNivel(alturaUltimoAndar, blocos, &numBlocos);

                    if (numMoedas < MAX_MOEDAS) {
                        moedas[numMoedas].rect = (Rectangle){ GetRandomValue(100, 600), alturaUltimoAndar + 30, 20, 20 };
                        moedas[numMoedas].coletada = false;
                        numMoedas++;
                    }

                    if ((int)(alturaUltimoAndar / 100) % 3 == 0 && GetRandomValue(0, 100) < 50) {
                        GerarInimigo(alturaUltimoAndar);
                    }

                } else {
                    float yBase = alturaUltimoAndar;
                    float xBase[] = {280, 330, 380, 430};

                    for (int i = 0; i < 4; i++) {
                        blocos[numBlocos++] = (Rectangle){ xBase[i], yBase, 50, 10 };
                    }

                    baseFinalCriada = true;
                }
            }

            // Vitória
            if (baseFinalCriada && !venceu) {
                for (int i = 0; i < numBlocos; i++) {
                    if (blocos[i].y == alturaUltimoAndar) {
                        Rectangle jogador = { posJogador.x, posJogador.y, PLAYER_SIZE, PLAYER_SIZE };
                        if (CheckCollisionRecs(jogador, blocos[i])) {
                            tempoFinal = (int)(GetTime() - tempoInicio);
                            venceu = true;
                            int moedasFinais = (escolha == 1) ? player1.moedas : player2.moedas;
                            SalvarRanking(moedasFinais, tempoFinal);
                            break;
                        }
                    }
                }
            }

            if (escolha == 1) UpdateControles(&player1, 1);
            if (escolha == 2) UpdateControles(&player2, 2);

            // Desenhar moedas
            for (int i = 0; i < numMoedas; i++) {
                if (!moedas[i].coletada) {
                    DrawRectangleRec(moedas[i].rect, YELLOW);
                }
            }

            // Desenhar inimigos
            for (int i = 0; i < numInimigos; i++) {
                if (inimigos[i].ativo) {
                    DrawRectangleRec(inimigos[i].rect, RED);
                }
            }

            // Desenhar jogador
            if (escolha == 1) DesenharJogador(player1);
            if (escolha == 2) DesenharJogador(player2);

            EndMode2D();

            // Mensagem de vitória
            if (venceu) {
                char texto[100];
                sprintf(texto, "Parabéns! Tempo: %d segundos", tempoFinal);
                DrawText(texto, 200, 100, 30, RED);
                DrawText("Pressione ENTER para voltar ao menu", 200, 150, 20, DARKBLUE);

                if (IsKeyPressed(KEY_ENTER)) {
                    escolha = 0;
                }
            }
        }

        EndDrawing();
    }

    LiberarTexturas(player1, player2, ground, bloco, cano);
    CloseWindow();
    return 0;
}

// ===================================
// Funções auxiliares
// ===================================

void ResetarJogo(Player *player1, Player *player2, Rectangle *blocos, int *numBlocos, float *alturaUltimoAndar, bool *baseFinalCriada, double *tempoInicio) {
    InitPlayers(player1, player2);
    player1->moedas = 0;
    player2->moedas = 0;

    *alturaUltimoAndar = ALTURA - GROUND_ALTURA - PLAYER_SIZE - 100;
    *baseFinalCriada = false;

    *numBlocos = 0;
    numMoedas = 0;
    numInimigos = 0;

    GerarBlocosNivel(*alturaUltimoAndar, blocos, numBlocos);
    *tempoInicio = GetTime();
}

void GerarInimigo(float y) {
    if (numInimigos < MAX_INIMIGOS) {
        inimigos[numInimigos].rect = (Rectangle){ GetRandomValue(100, 600), y + 30, 20, 20 };
        inimigos[numInimigos].ativo = true;
        numInimigos++;
    }
}

void SalvarRanking(int moedas, int tempo) {
    FILE *arquivo = fopen("ranking.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d %d\n", moedas, tempo);
        fclose(arquivo);
    }
}

void MostrarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) {
        DrawText("Sem ranking disponível", 250, 300, 20, DARKGRAY);
        return;
    }

    int moedas[100];
    int tempos[100];
    int count = 0;

    while (fscanf(arquivo, "%d %d", &moedas[count], &tempos[count]) == 2 && count < 100) {
        count++;
    }
    fclose(arquivo);

    // Ordenar pelo menor tempo
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (tempos[j] < tempos[i]) {
                int tempT = tempos[i];
                tempos[i] = tempos[j];
                tempos[j] = tempT;

                int tempM = moedas[i];
                moedas[i] = moedas[j];
                moedas[j] = tempM;
            }
        }
    }

    int y = 300;
    for (int i = 0; i < count && i < 5; i++) {
        char texto[200];
        sprintf(texto, "%dº lugar - %ds - %d moedas", i + 1, tempos[i], moedas[i]);
        DrawText(texto, 200, y, 20, DARKBLUE);
        y += 30;
    }
}
