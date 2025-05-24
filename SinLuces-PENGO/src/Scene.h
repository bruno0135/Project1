#pragma once
#include <raylib.h>
#include "Enemy.h"
#include "AABB.h"
#include "Snobee.h"
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "EnemyManager.h"
#include "ShotManager.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

enum class SceneState { PLAYING, WIN, LOSE };

class Scene
{
public:

    SceneState scene_state = SceneState::PLAYING;

    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

private:
    AppStatus LoadLevel(int stage);


    void CheckObjectCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;


    Player* player;

    //Level structure that contains all the static tiles
    TileMap* level;

    //Dynamic objects of the level: items and collectables
    std::vector<Object*> objects;

    //Enemies present in the level
    EnemyManager* enemies;

    //Shots thrown by enemies
    ShotManager* shots;

    Camera2D camera;
    DebugMode debug;

    bool diamondLineDetected = false;   // Si s'ha detectat línia de diamants
    float diamondCooldown = 0.5f;       // Temps de cooldown en segons (per exemple 2 segons)
    float diamondTimer = 0.0f;          // Temporitzador acumulat

};
