#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "game.h"

#define MAX_BLOCOS 1000
#define MAX_MOEDAS 100
#define MAX_INIMIGOS 50

// ===== Lista encadeada =====
typedef struct NodoRanking {
    int moedas;
    int tempo;
    struct NodoRanking *prox;
} NodoRanking;

// ===== Moeda e Inimigo =====
typedef struct {
    Rectangle rect;
    bool coletada;
} Moeda;

typedef struct {
    Rectangle rect;
    bool ativo;
} Inimigo;

// Variáveis globais
Moeda moedas[MAX_MOEDAS];
int numMoedas = 0;

Inimigo inimigos[MAX_INIMIGOS];
int numInimigos = 0;

NodoRanking *rankingHead = NULL;

// Funções auxiliares
void SalvarRanking(int moedasColetadas, int tempo);
void MostrarRanking();
void GerarInimigo(float y);
void ResetarJogo(Player *player1, Player *player2, Rectangle *blocos, int *numBlocos, float *alturaUltimoAndar, bool *baseFinalCriada, double *tempoInicio);

int main(void) {
    InitWindow(LARGURA, ALTURA, "The Plumber Duo");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    Player player1, player2;
    Texture2D ground, bloco, cano;
    CarregarTexturas(&player1, &player2, &ground, &bloco, &cano);
    InitPlayers(&player1, &player2);

    // ===== Alocacao dinamica dos blocos =====
    Rectangle *blocos = (Rectangle *)malloc(sizeof(Rectangle) * MAX_BLOCOS);
    int numBlocos = 0;

    // ===== Matriz exemplo (alocacao dinamica) =====
    int **matrizMapa = (int **)malloc(10 * sizeof(int *));
    for (int i = 0; i < 10; i++) {
        matrizMapa[i] = (int *)malloc(10 * sizeof(int));
        for (int j = 0; j < 10; j++) matrizMapa[i][j] = 0;
    }

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
                } else if (menuIndex == 3) break;
            }

        } else {
            BeginMode2D(camera);

            UpdateFisica(&player1, blocos, numBlocos);
            UpdateFisica(&player2, blocos, numBlocos);
            DesenharCenario(ground, blocos, numBlocos, bloco, cano);

            Vector2 posJogador = (escolha == 1) ? player1.pos : player2.pos;
            AtualizarCamera(&camera, posJogador);

            Rectangle jogador = { posJogador.x, posJogador.y, PLAYER_SIZE, PLAYER_SIZE };

            for (int i = 0; i < numMoedas; i++) {
                if (!moedas[i].coletada && CheckCollisionRecs(jogador, moedas[i].rect)) {
                    moedas[i].coletada = true;
                    if (escolha == 1) player1.moedas++;
                    else player2.moedas++;
                }
            }

            for (int i = 0; i < numInimigos; i++) {
                if (inimigos[i].ativo && CheckCollisionRecs(jogador, inimigos[i].rect)) {
                    escolha = 0;
                    break;
                }
            }

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
                    float xBase[] = {280, 330, 380, 430};
                    for (int i = 0; i < 4; i++) {
                        blocos[numBlocos++] = (Rectangle){ xBase[i], alturaUltimoAndar, 50, 10 };
                    }
                    baseFinalCriada = true;
                }
            }

            if (baseFinalCriada && !venceu) {
                for (int i = 0; i < numBlocos; i++) {
                    if (blocos[i].y == alturaUltimoAndar && CheckCollisionRecs(jogador, blocos[i])) {
                        tempoFinal = (int)(GetTime() - tempoInicio);
                        venceu = true;
                        int moedasFinais = (escolha == 1) ? player1.moedas : player2.moedas;
                        SalvarRanking(moedasFinais, tempoFinal);
                        break;
                    }
                }
            }

            if (escolha == 1) UpdateControles(&player1, 1);
            if (escolha == 2) UpdateControles(&player2, 2);

            for (int i = 0; i < numMoedas; i++) if (!moedas[i].coletada) DrawRectangleRec(moedas[i].rect, YELLOW);
            for (int i = 0; i < numInimigos; i++) if (inimigos[i].ativo) DrawRectangleRec(inimigos[i].rect, RED);

            if (escolha == 1) DesenharJogador(player1);
            if (escolha == 2) DesenharJogador(player2);

            EndMode2D();

            if (venceu) {
                char texto[100];
                sprintf(texto, "Parabéns! Tempo: %d segundos", tempoFinal);
                DrawText(texto, 200, 100, 30, RED);
                DrawText("Pressione ENTER para voltar ao menu", 200, 150, 20, DARKBLUE);
                if (IsKeyPressed(KEY_ENTER)) escolha = 0;
            }
        }

        EndDrawing();
    }

    LiberarTexturas(player1, player2, ground, bloco, cano);
    free(blocos);
    for (int i = 0; i < 10; i++) free(matrizMapa[i]);
    free(matrizMapa);
    CloseWindow();
    return 0;
}

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
    NodoRanking *novo = malloc(sizeof(NodoRanking));
    novo->moedas = moedas;
    novo->tempo = tempo;
    novo->prox = rankingHead;
    rankingHead = novo;

    FILE *arquivo = fopen("ranking.txt", "a");
    if (arquivo) {
        fprintf(arquivo, "%d %d\n", moedas, tempo);
        fclose(arquivo);
    }
}

void MostrarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (!arquivo) {
        DrawText("Sem ranking disponível", 250, 300, 20, DARKGRAY);
        return;
    }
    int moedas, tempo, y = 300, pos = 1;
    while (fscanf(arquivo, "%d %d", &moedas, &tempo) == 2 && pos <= 5) {
        char texto[200];
        sprintf(texto, "%dº lugar - %ds - %d moedas", pos++, tempo, moedas);
        DrawText(texto, 200, y, 20, DARKBLUE);
        y += 30;
    }
    fclose(arquivo);
}
