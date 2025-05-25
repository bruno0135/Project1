#pragma once
#include "Enemy.h"
#include "TileMap.h"
#include "Player.h"   // Canviat aquí per Player

#define SNOBEE_SPEED         1
#define SNOBEE_ANIM_DELAY    (2*ANIM_DELAY)
#define DETECTION_RADIUS     100
#define MIN_DISTANCE_TO_PLAYER 10

enum class SNOBEEState { ROAMING, CHASING, ATTACK };
enum class SNOBEEAnim {
    IDLE_LEFT, IDLE_RIGHT, WALKING_LEFT, WALKING_RIGHT,
    ATTACK_LEFT, ATTACK_RIGHT, NUM_ANIMATIONS
};

struct Step
{
    Point speed;    //direction
    int frames;     //duration in number of frames
    int anim;       //graphical representation
};

class SNOBEE : public Enemy
{
public:
    SNOBEE(const Point& p, int width, int height, int frame_width, int frame_height);
    ~SNOBEE();

    AppStatus Initialise(const Point& pos, EnemyType type, const AABB& area, TileMap* map) override;

    bool Update(const AABB& playerBox) override;

    // Funció per intentar atacar el jugador
    bool TryAttack(const AABB& playerBox, Player& player);

    TileMap* map = nullptr;

private:
    void InitPattern();

    // Mètodes nous
    void RoamingMovement(int baseSpeed);
    void ChasingMovement(const Point& playerCenter, const Point& enemyCenter, int baseSpeed);
    void StepMovementWithCollision();
    bool CanMove(const Point& delta);

    int attack_delay;
    SNOBEEState state;

    int stepsRemaining = 0;
    Point movement = { 0, 0 };

    int current_step;
    int current_frames;
    std::vector<Step> pattern;
};
