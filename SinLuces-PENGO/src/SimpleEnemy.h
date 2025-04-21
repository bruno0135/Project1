#pragma once
#pragma once
#include "raylib.h"

#define SIMPLE_ENEMY_SIZE 16
#define SIMPLE_ENEMY_SPEED 1

typedef struct SimpleEnemy {
    Rectangle rect;
    Vector2 speed;
    bool active;
} SimpleEnemy;

void InitSimpleEnemy(SimpleEnemy* enemy, Vector2 pos);
void UpdateSimpleEnemy(SimpleEnemy* enemy);
void DrawSimpleEnemy(SimpleEnemy* enemy);