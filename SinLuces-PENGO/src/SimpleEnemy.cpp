#include "SimpleEnemy.h"

void InitSimpleEnemy(SimpleEnemy* enemy, Vector2 pos)
{
    enemy->rect.x = pos.x;
    enemy->rect.y = pos.y;
    enemy->rect.width = SIMPLE_ENEMY_SIZE;
    enemy->rect.height = SIMPLE_ENEMY_SIZE;

    enemy->speed = (Vector2){ SIMPLE_ENEMY_SPEED, 0 };
    enemy->active = true;
}

void UpdateSimpleEnemy(SimpleEnemy* enemy)
{
    if (!enemy->active) return;

    enemy->rect.x += enemy->speed.x;

    if (enemy->rect.x <= 0 || enemy->rect.x + enemy->rect.width >= GetScreenWidth())
    {
        enemy->speed.x *= -1;
    }
}

void DrawSimpleEnemy(SimpleEnemy* enemy)
{
    if (!enemy->active) return;

    DrawRectangleRec(enemy->rect, RED);
}