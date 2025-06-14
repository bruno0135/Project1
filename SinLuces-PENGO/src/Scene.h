﻿#pragma once
#include <raylib.h>
#include <vector>
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

    explicit Scene(ResourceManager& data);
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    void SetFont(Font* font);

    // ✅ AÑADIDO: Acceso al estado de la escena (para Game.cpp)
    SceneState GetState() const { return scene_state; }

    // ✅ AÑADIDO: Acceso al TileMap para poder usar GenerateRandomMap()
    TileMap* GetTileMap() const { return level; }

private:
    AppStatus LoadLevel(int stage);
    ResourceManager& resMan;

    void CheckObjectCollisions();
    void CheckPlayerEnemyCollisions();

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
    Font* customFont = nullptr; // ✅ INICIALIZADA

    Camera2D camera;
    DebugMode debug;

    bool godMode = false;

    bool diamondLineDetected = false;
    float diamondCooldown = 0.5f;
    float diamondTimer = 0.0f;
};
