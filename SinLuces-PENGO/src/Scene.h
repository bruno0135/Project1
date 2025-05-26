#pragma once
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

    // Constructor únic, obligatori passar ResourceManager
    explicit Scene(ResourceManager& data);
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

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
    TileMap* level;
    std::vector<Object*> objects;
    EnemyManager* enemies;
    ShotManager* shots;

    Camera2D camera;
    DebugMode debug;

    bool godMode = false;

    bool diamondLineDetected = false;
    float diamondCooldown = 0.5f;
    float diamondTimer = 0.0f;
};
