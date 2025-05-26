
#include "Snobee.h"
#include "Sprite.h"
#include "Player.h"

#include <cmath>

#define DETECTION_RADIUS 100
#define MIN_DISTANCE_TO_PLAYER 10

static float Distance(const Point& a, const Point& b)
{
    float dx = float(a.x - b.x);
    float dy = float(a.y - b.y);
    return std::sqrt(dx * dx + dy * dy);
}

SNOBEE::~SNOBEE() = default;

SNOBEE::SNOBEE(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height)
{
    attack_delay = 1;
    state = SNOBEEState::ROAMING;

    current_step = 0;
    current_frames = 0;
    stepsRemaining = 0;
    movement = { 0, 0 };
}

AppStatus SNOBEE::Initialise(const Point& pos, EnemyType type, const AABB& area, TileMap* map)
{
    this->pos = pos;
    this->map = map;
    this->visibility_area = area;
    this->look = Look::RIGHT;

    state = SNOBEEState::ROAMING;

    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
    if (render == nullptr)
    {
        LOG("Failed to allocate memory for sno-bee sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)SNOBEEAnim::NUM_ANIMATIONS);

    const int n = SNOBEE_FRAME_SIZE;

    sprite->SetAnimationDelay((int)SNOBEEAnim::IDLE_RIGHT, SNOBEE_ANIM_DELAY);
    sprite->AddKeyFrame((int)SNOBEEAnim::IDLE_RIGHT, { 1, 7 * n, n, n });
    sprite->SetAnimationDelay((int)SNOBEEAnim::IDLE_LEFT, SNOBEE_ANIM_DELAY);
    sprite->AddKeyFrame((int)SNOBEEAnim::IDLE_LEFT, { 3 * n, n, n, n });

    sprite->SetAnimationDelay((int)SNOBEEAnim::WALKING_RIGHT, SNOBEE_ANIM_DELAY);
    for (int i = 0; i < 3; ++i)
        sprite->AddKeyFrame((int)SNOBEEAnim::WALKING_RIGHT, { (float)i * n, 2 * n, n, n });
    sprite->SetAnimationDelay((int)SNOBEEAnim::WALKING_LEFT, SNOBEE_ANIM_DELAY);
    for (int i = 0; i < 3; ++i)
        sprite->AddKeyFrame((int)SNOBEEAnim::WALKING_LEFT, { (float)i * n, 2 * n, -n, n });

    sprite->SetAnimationDelay((int)SNOBEEAnim::ATTACK_RIGHT, SNOBEE_ANIM_DELAY);
    sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_RIGHT, { 0, 3 * n, n, n });
    sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_RIGHT, { n, 3 * n, n, n });
    sprite->SetAnimationDelay((int)SNOBEEAnim::ATTACK_LEFT, SNOBEE_ANIM_DELAY);
    sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_LEFT, { 0, 3 * n, -n, n });
    sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_LEFT, { n, 3 * n, -n, n });

    if (look == Look::LEFT)        SetAnimation((int)SNOBEEAnim::IDLE_LEFT);
    else if (look == Look::RIGHT) SetAnimation((int)SNOBEEAnim::IDLE_RIGHT);

    visibility_area = area;

    InitPattern();

    return AppStatus::OK;
}

void SNOBEE::InitPattern()
{
   
}

bool SNOBEE::Update(const AABB& playerBox)
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);


    Point enemyCenter = { pos.x + width / 2, pos.y + height / 2 };
    Point playerCenter = { playerBox.pos.x + playerBox.width / 2, playerBox.pos.y + playerBox.height / 2 };
    float dist = Distance(enemyCenter, playerCenter);

    int baseSpeed = std::max(1, SNOBEE_SPEED / 2);

    switch (state)
    {
    case SNOBEEState::ROAMING:
    {
        if (dist <= DETECTION_RADIUS)
        {
            state = SNOBEEState::CHASING;
            break;
        }
        RoamingMovement(baseSpeed);
        break;
    }
    case SNOBEEState::CHASING:
    {
        if (dist > DETECTION_RADIUS)
        {
            state = SNOBEEState::ROAMING;
            movement = { 0,0 };
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
            break;
        }
        ChasingMovement(playerCenter, enemyCenter, baseSpeed);
        break;
    }

    case SNOBEEState::ATTACK:
     
        break;
    }

    StepMovementWithCollision();


    AABB nextPosHitbox = GetHitbox();
    if (nextPosHitbox.TestAABB(playerBox))
    {
        movement = { 0,0 };
        SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
    }
    sprite->Update();

    return false;
}

void SNOBEE::RoamingMovement(int baseSpeed)
{
    if (stepsRemaining <= 0)
    {
        int dir = GetRandomValue(0, 3);
        Point dirVec = { 0,0 };
        switch (dir)
        {
        case 0: dirVec.x = -baseSpeed; look = Look::LEFT; break;
        case 1: dirVec.x = baseSpeed;  look = Look::RIGHT; break;
        case 2: dirVec.y = -baseSpeed; break;
        case 3: dirVec.y = baseSpeed;  break;
        }

        int tileSize = SNOBEE_FRAME_SIZE;
        movement = { dirVec.x, dirVec.y };
        stepsRemaining = (2 * tileSize) / baseSpeed;
        current_frames = 0;
    }
    else
    {
        current_frames++;
        stepsRemaining--;


        if (!CanMove(movement))

        {
            stepsRemaining = 0;
            movement = { 0,0 };            
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
            return;
        }
        SetAnimation((movement.x < 0) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);

    }
}

void SNOBEE::ChasingMovement(const Point& playerCenter, const Point& enemyCenter, int baseSpeed)
{
    Point direction = { 0,0 };
    if (std::abs(playerCenter.x - enemyCenter.x) > std::abs(playerCenter.y - enemyCenter.y))
    {
        direction.x = (playerCenter.x < enemyCenter.x) ? -baseSpeed : baseSpeed;
        look = (direction.x < 0) ? Look::LEFT : Look::RIGHT;
    }
    else 
    {
        direction.y = (playerCenter.y < enemyCenter.y) ? -baseSpeed : baseSpeed;
    }
    if (CanMove(direction))
        movement = direction;
    else if (CanMove({ direction.x, 0 }))
        movement = { direction.x, 0 };
    else if (CanMove({ 0, direction.y }))
        movement = { 0, direction.y };
    else
        movement = { 0, 0 };
    if (movement.x == 0 && movement.y == 0)
        SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
    
    else
        SetAnimation((movement.x < 0) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
}

bool SNOBEE::CanMove(const Point& delta)
{
    AABB projected = GetHitbox();
    projected.pos.x += delta.x;
    projected.pos.y += delta.y;

    bool canMove = true;

    if (delta.x < 0)
        canMove = !map->TestCollisionWallLeft(projected);
    else if (delta.x > 0)
        canMove = !map->TestCollisionWallRight(projected);

    if (canMove && delta.y != 0)
    {
        if (delta.y < 0)
            canMove = !map->TestCollisionWallUp(projected);
        else
            canMove = !map->TestCollisionWallDown(projected);
    }

    return canMove;
}
void SNOBEE::StepMovementWithCollision()
{
    AABB currentHitbox = GetHitbox();

    int steps = std::max(std::max(abs(movement.x), abs(movement.y)), 1);

    Point stepMove = { movement.x / steps, movement.y / steps };

    for (int i = 0; i < steps; i++)
    {
        AABB nextHitbox = currentHitbox;
        nextHitbox.pos.x += stepMove.x;
        nextHitbox.pos.y += stepMove.y;

        if (CanMove({ stepMove.x, 0 }) && CanMove({ 0, stepMove.y }))
        {
            pos.x += stepMove.x;
            pos.y += stepMove.y;
            currentHitbox = nextHitbox;
        }
        else
        {
            movement = { 0,0 };
            break;
        }
    }
}
