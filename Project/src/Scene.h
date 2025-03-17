#pragma once
#include <raylib.h>
#include "Game.h"
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Globals.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "NavMesh.h"
#include "Puntuation.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SPRITES_AND_NAVMESH_ROUTES, SPRITES_AND_NAVMESH, SIZE };

class Game;

class Scene
{
public:
    Scene(Game* game, int startPuntuation, int startLives, int level);
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    void GoNextLevel();

private:
    AppStatus LoadLevel(int stage);

    void CheckEndLevel();
    void CheckCollisions();
    void UpdateGhostState();
    void PlaySounds();
    void StopSoundsInException(const Sound* s);
    void StopAllSounds();
    void ShowPuntuation(Point position, Puntuations puntuation);
    void EatGhostPuntuation(Point position);
    void EatFruitPuntuation(ObjectType type, Point position);
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void PlayerDie();
    void Win();
    void Won();
    void ReloadScene();


    void RenderGUI() const;

    //REFERENCES
    Player* player;
    Blinky* blinky;
    Pinky* pinky;
    Inky* inky;
    Clyde* clyde;
    TileMap* level;
    NavMesh* navMesh;
    Game* game;
    Puntuation* puntuation1;
    Puntuation* puntuation2;
    std::vector<Object*> objects;

    //VARIABLE
    int startPuntuation;
    int startLives;
    int lvl;
    int peletsCollected;
    int totalPelets;
    int munch;
    bool started;
    GhostState ghostState;
    float lastStateChangeTime; 
    float startWaitTime;
    float waitTime;
    float startWinTimer;
    float timer;
    int ghostEaten;
    bool returnMainMenu;
    bool goNextLevel;
    bool died;
    bool won;
    bool fruitSpawned;
    Point pacmanSpawnPos;
    Point ghostSpawnPos; 
    Point fruitSpawnPoint; 

    //SOUNDS
    const Sound* startMusic;
    const Sound* retreating;
    const Sound* power_pellet;
    const Sound* siren_1;
    const Sound* siren_2;
    const Sound* siren_3;
    const Sound* siren_4;
    const Sound* siren_5;
    const Sound* munch_1;
    const Sound* munch_2;
    const Sound* eat_ghost;
    const Sound* dead;
    const Sound* eat_fruit;

    //TEXTURES
    const Texture* lettersTexture;

    Camera2D camera;
    DebugMode debug;
};
