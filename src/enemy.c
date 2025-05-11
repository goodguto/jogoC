#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include "cjson.h" 

Enemy CarregarInimigo(const char *jsonPath) {
    Enemy enemy = {0};
    enemy.pos = (Vector2){ 200, 100 };
    enemy.frameAtual = 0;
    enemy.tempoFrame = 0;

    FILE *file = fopen(jsonPath, "r");
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(data);
    cJSON *walk = cJSON_GetObjectItemCaseSensitive(root, "enemy1_walk");
    cJSON *right = cJSON_GetObjectItemCaseSensitive(walk, "right");

    int frameIndex = 0;
    cJSON *frame = NULL;
    cJSON_ArrayForEach(frame, right) {
        const char *path = frame->valuestring;
        enemy.frames[frameIndex++] = LoadTexture(path);
    }

    enemy.frameCount = frameIndex;

    cJSON *frame_change = cJSON_GetObjectItemCaseSensitive(root, "frame_change");
    enemy.intervalo = cJSON_IsNumber(frame_change) ? (float)frame_change->valuedouble : 0.1f;

    cJSON_Delete(root);
    free(data);



    return enemy;
}

void AtualizarInimigo(Enemy *e) {
    e->tempoFrame += GetFrameTime();
    if (e->tempoFrame >= e->intervalo) {
        e->frameAtual = (e->frameAtual + 1) % e->frameCount;
        e->tempoFrame = 0;
    }
}

void DesenharInimigo(Enemy e) {
    DrawTexture(e.frames[e.frameAtual], e.pos.x, e.pos.y, WHITE);
}

void LiberarInimigo(Enemy *e) {
    for (int i = 0; i < e->frameCount; i++) {
        UnloadTexture(e->frames[i]);
    }
}
