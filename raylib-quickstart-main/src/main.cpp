/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "raylib.h" const int screenWidth = 800; const int screenHeight = 600; // Estructura para el personaje Pengo struct Pengo { Vector2 position; float speed; Color color; void Move() { if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed; if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) position.x -= speed; if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) position.y += speed; if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) position.y -= speed; // Limitar movimiento dentro de la pantalla if (position.x < 0) position.x = 0; if (position.y < 0) position.y = 0; if (position.x > screenWidth - 40) position.x = screenWidth - 40; if (position.y > screenHeight - 40) position.y = screenHeight - 40; } void Draw() { DrawRectangle(position.x, position.y, 40, 40, color); } }; int main() { InitWindow(screenWidth, screenHeight, "Pengo en Raylib"); SetTargetFPS(60); Pengo pengo = {{screenWidth / 2.0f, screenHeight / 2.0f}, 5.0f, BLUE}; while (!WindowShouldClose()) { BeginDrawing(); ClearBackground(RAYWHITE); pengo.Move(); pengo.Draw(); DrawText("Mueve a Pengo con WASD o Flechas", 10, 10, 20, DARKGRAY); EndDrawing(); } CloseWindow(); return 0; }

