//#include "Snobee.h"
//#include "Sprite.h"
//
//
//SNOBEE::~SNOBEE() = default;
//
//SNOBEE::SNOBEE(const Point& p, int width, int height, int frame_width, int frame_height) :
//	Enemy(p, width, height, frame_width, frame_height)
//{
//	attack_delay = 0;
//	state = SNOBEEState::ROAMING;
//
//	current_step = 0;
//	current_frames = 0;
//}
//AppStatus SNOBEE::Initialise(const Point& pos, EnemyType type, const AABB& area, TileMap* map)
//{
//	this->pos = pos;
//	this->map = map;
//	this->visibility_area = area;
//	this->look = Look::RIGHT; // o LEFT, o según `type` si quieres variarlo
//
//	state = SNOBEEState::ROAMING;
//	this->map = map;
//
//	int i;
//	const int n = SNOBEE_FRAME_SIZE;
//
//	ResourceManager& data = ResourceManager::Instance();
//	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
//	if (render == nullptr)
//	{
//		LOG("Failed to allocate memory for sno-bee sprite");
//		return AppStatus::ERROR;
//	}
//
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetNumberAnimations((int)SNOBEEAnim::NUM_ANIMATIONS);
//
//	sprite->SetAnimationDelay((int)SNOBEEAnim::IDLE_RIGHT, SNOBEE_ANIM_DELAY);
//	sprite->AddKeyFrame((int)SNOBEEAnim::IDLE_RIGHT, { 1, 7*n, n, n });
//	sprite->SetAnimationDelay((int)SNOBEEAnim::IDLE_LEFT, SNOBEE_ANIM_DELAY);
//	sprite->AddKeyFrame((int)SNOBEEAnim::IDLE_LEFT, { 3*n, n, n, n});
//
//	sprite->SetAnimationDelay((int)SNOBEEAnim::WALKING_RIGHT, SNOBEE_ANIM_DELAY);
//	for (i = 0; i < 3; ++i)
//		sprite->AddKeyFrame((int)SNOBEEAnim::WALKING_RIGHT, { (float)i*n, 2*n, n, n });
//	sprite->SetAnimationDelay((int)SNOBEEAnim::WALKING_LEFT, SNOBEE_ANIM_DELAY);
//	for (i = 0; i < 3; ++i)
//		sprite->AddKeyFrame((int)SNOBEEAnim::WALKING_LEFT, { (float)i*n, 2*n, -n, n });
//
//	sprite->SetAnimationDelay((int)SNOBEEAnim::ATTACK_RIGHT, SNOBEE_ANIM_DELAY);
//	sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_RIGHT, { 0, 3*n, n, n });
//	sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_RIGHT, { n, 3*n, n, n });
//	sprite->SetAnimationDelay((int)SNOBEEAnim::ATTACK_LEFT, SNOBEE_ANIM_DELAY);
//	sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_LEFT, { 0, 3*n, -n, n });
//	sprite->AddKeyFrame((int)SNOBEEAnim::ATTACK_LEFT, { n, 3*n, -n, n });
//
//	this->look = look;
//	if(look == Look::LEFT)        SetAnimation((int)SNOBEEAnim::IDLE_LEFT);
//	else if (look == Look::RIGHT) SetAnimation((int)SNOBEEAnim::IDLE_RIGHT);
//	
//	visibility_area = area;
//
//	InitPattern();
//
//	return AppStatus::OK;
//}
//void SNOBEE::InitPattern()
//{
//	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
//	//walking animation, resulting in smoother transitions and preventing the animation
//	//from appearing rushed or incomplete
//	const int n = SNOBEE_ANIM_DELAY*3;
//
//	pattern.push_back({ {0, 0}, 2*n, (int)SNOBEEAnim::IDLE_RIGHT });
//	pattern.push_back({ {SNOBEE_SPEED, 0}, n, (int)SNOBEEAnim::WALKING_RIGHT });
//	pattern.push_back({ {0, 0}, n, (int)SNOBEEAnim::IDLE_RIGHT });
//	pattern.push_back({ {SNOBEE_SPEED, 0}, n, (int)SNOBEEAnim::WALKING_RIGHT });
//	pattern.push_back({ {0, 0}, n, (int)SNOBEEAnim::IDLE_RIGHT });
//	
//	pattern.push_back({ {0, 0}, 2*n, (int)SNOBEEAnim::IDLE_LEFT });
//	pattern.push_back({ {-SNOBEE_SPEED, 0}, n, (int)SNOBEEAnim::WALKING_LEFT });
//	pattern.push_back({ {0, 0}, n, (int)SNOBEEAnim::IDLE_LEFT });
//	pattern.push_back({ {-SNOBEE_SPEED, 0}, n, (int)SNOBEEAnim::WALKING_LEFT });
//	pattern.push_back({ {0, 0}, n, (int)SNOBEEAnim::IDLE_LEFT });
//	
//	current_step = 0;
//	current_frames = 0;
//}
//
//
//
//
//void SNOBEE::UpdateMovementAI(const AABB& playerBox)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	const int tileSize = 16;
//	int baseSpeed = std::max(1, SNOBEE_SPEED / 2);
//
//	if (stepsRemaining == 0)
//	{
//		// Elegir dirección hacia el jugador o aleatoria
//		Point direction = { 0, 0 };
//		if (IsVisible(playerBox))
//		{
//			Point playerPos = playerBox.pos;
//			if (abs(playerPos.x - pos.x) > abs(playerPos.y - pos.y))
//			{
//				direction.x = (playerPos.x < pos.x) ? -baseSpeed : baseSpeed;
//				look = (direction.x < 0) ? Look::LEFT : Look::RIGHT;
//			}
//			else
//			{
//				direction.y = (playerPos.y < pos.y) ? -baseSpeed : baseSpeed;
//			}
//		}
//		else
//		{
//			// Movimiento aleatorio
//			int dir = GetRandomValue(0, 3);
//			switch (dir)
//			{
//			case 0: direction = { -baseSpeed, 0 }; look = Look::LEFT; break;
//			case 1: direction = { baseSpeed, 0 };  look = Look::RIGHT; break;
//			case 2: direction = { 0, -baseSpeed }; break;
//			case 3: direction = { 0, baseSpeed };  break;
//			}
//		}
//
//		// Proyectar movimiento
//		AABB projected = GetHitbox();
//		projected.pos += direction;
//
//		// Comprobar colisión con el hitbox proyectado
//		bool blocked = false;
//		if (direction.x < 0) blocked = map->TestCollisionWallLeft(projected);
//		else if (direction.x > 0) blocked = map->TestCollisionWallRight(projected);
//		else if (direction.y < 0)
//		{
//			// Asegurar que proyectamos correctamente hacia arriba
//			AABB adjusted = projected;
//			adjusted.pos.y -= 1;
//			blocked = map->TestCollisionWallUp(adjusted);
//		}
//
//		else if (direction.y > 0) blocked = map->TestCollisionWallDown(projected);
//
//		if (blocked)
//		{
//			movement = { 0, 0 };
//			stepsRemaining = 0;
//			SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
//			return;
//		}
//
//		// Movimiento válido
//		movement = direction;
//		stepsRemaining = tileSize / baseSpeed;
//		if (movement.x != 0)
//		{
//			SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
//		}
//		else
//		{
//			SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
//		}
//	}
//
//	pos += movement;
//	stepsRemaining--;
//
//	if (stepsRemaining == 0)
//	{
//		SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
//	}
//
//	sprite->Update();
//}
//
//
//void SNOBEE::MoveOneTileInDirection(Look dir)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//
//	const int tileSize = 16;
//	int baseSpeed = std::max(1, SNOBEE_SPEED / 2);  // Protección contra 0
//
//	if (stepsRemaining == 0) {
//		switch (dir) {
//		case Look::LEFT:
//			movement = { -baseSpeed, 0 };
//			SetAnimation((int)SNOBEEAnim::WALKING_LEFT);
//			break;
//		case Look::RIGHT:
//			movement = { baseSpeed, 0 };
//			SetAnimation((int)SNOBEEAnim::WALKING_RIGHT);
//			break;
//		case Look::UP:
//			movement = { 0, -baseSpeed };
//			break;
//		case Look::DOWN:
//			movement = { 0, baseSpeed };
//			break;
//		}
//
//		stepsRemaining = tileSize / baseSpeed;
//		look = dir;
//	}
//
//	pos += movement;
//	stepsRemaining--;
//
//	if (stepsRemaining == 0) {
//		if (look == Look::LEFT)
//			SetAnimation((int)SNOBEEAnim::IDLE_LEFT);
//		else if (look == Look::RIGHT)
//			SetAnimation((int)SNOBEEAnim::IDLE_RIGHT);
//	}
//
//	sprite->Update();
//}
//bool SNOBEE::Update(const AABB& box)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	bool shoot = false;
//	int anim_id;
//
//	// NUEVO: movimiento por IA (aleatorio o hacia el jugador)
//	UpdateMovementAI(box);
//
//	// Mantenimiento de estado ATTACK (mantiene animación y delay)
//	if (state == SNOBEEState::ATTACK)
//	{
//		if (!IsVisible(box))
//		{
//			state = SNOBEEState::ROAMING;
//			anim_id = (look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT;
//			sprite->SetAnimation(anim_id);
//		}
//		else
//		{
//			attack_delay--;
//			if (attack_delay == 0)
//			{
//				// Acción de ataque (a implementar si quieres disparo u otro efecto)
//				attack_delay = 2 * SNOBEE_ANIM_DELAY;
//				shoot = true;
//			}
//		}
//	}
//	else if (state == SNOBEEState::ROAMING)
//	{
//		if (IsVisible(box))
//		{
//			state = SNOBEEState::ATTACK;
//			attack_delay = SNOBEE_ANIM_DELAY;
//			SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::ATTACK_LEFT : (int)SNOBEEAnim::ATTACK_RIGHT);
//		}
//	}
//
//	sprite->Update();
//	return shoot;
//}
//
//void SNOBEE::UpdateLook(int anim_id)
//{
//	SNOBEEAnim anim = (SNOBEEAnim)anim_id;
//	look = (anim == SNOBEEAnim::IDLE_LEFT ||
//			anim == SNOBEEAnim::WALKING_LEFT ||
//			anim == SNOBEEAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
//}
//------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------

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
    attack_delay = 0;
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
    // ...
    // (mantinc el teu codi original de InitPattern sense canvis)
}

bool SNOBEE::Update(const AABB& playerBox)
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);

    int baseSpeed = std::max(1, SNOBEE_SPEED / 2);

    Point enemyCenter = { pos.x + width / 2, pos.y + height / 2 };
    Point playerCenter = { playerBox.pos.x + playerBox.width / 2, playerBox.pos.y + playerBox.height / 2 };
    float dist = Distance(enemyCenter, playerCenter);

    movement = { 0, 0 };

    if (dist < MIN_DISTANCE_TO_PLAYER)
    {
        Point retreat = {
            (playerCenter.x < enemyCenter.x) ? baseSpeed : -baseSpeed,
            (playerCenter.y < enemyCenter.y) ? baseSpeed : -baseSpeed
        };

        bool canMoveX = true, canMoveY = true;

        AABB projectedX = GetHitbox();
        projectedX.pos.x += retreat.x;
        if (retreat.x < 0)
            canMoveX = !map->TestCollisionWallLeft(projectedX);
        else
            canMoveX = !map->TestCollisionWallRight(projectedX);

        AABB projectedY = GetHitbox();
        projectedY.pos.y += retreat.y;
        if (retreat.y < 0)
            canMoveY = !map->TestCollisionWallUp(projectedY);
        else
            canMoveY = !map->TestCollisionWallDown(projectedY);

        movement.x = canMoveX ? retreat.x : 0;
        movement.y = canMoveY ? retreat.y : 0;

        if (movement.x == 0 && movement.y == 0)
        {
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
            return false;
        }
        else
        {
            SetAnimation((movement.x < 0) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
        }
    }
    else if (dist <= DETECTION_RADIUS)
    {
        Point direction = { 0, 0 };
        if (std::abs(playerCenter.x - enemyCenter.x) > std::abs(playerCenter.y - enemyCenter.y))
        {
            direction.x = (playerCenter.x < enemyCenter.x) ? -baseSpeed : baseSpeed;
            look = (direction.x < 0) ? Look::LEFT : Look::RIGHT;
        }
        else
        {
            direction.y = (playerCenter.y < enemyCenter.y) ? -baseSpeed : baseSpeed;
        }

        bool canMoveX = true, canMoveY = true;

        AABB projectedX = GetHitbox();
        projectedX.pos.x += direction.x;
        if (direction.x < 0)
            canMoveX = !map->TestCollisionWallLeft(projectedX);
        else if (direction.x > 0)
            canMoveX = !map->TestCollisionWallRight(projectedX);

        AABB projectedY = GetHitbox();
        projectedY.pos.y += direction.y;
        if (direction.y < 0)
            canMoveY = !map->TestCollisionWallUp(projectedY);
        else
            canMoveY = !map->TestCollisionWallDown(projectedY);

        movement.x = canMoveX ? direction.x : 0;
        movement.y = canMoveY ? direction.y : 0;

        if (movement.x == 0 && movement.y == 0)
        {
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
            return false;
        }
        else
        {
            SetAnimation((movement.x < 0) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
        }
    }
    else
    {
        int dir = GetRandomValue(0, 3);
        Point direction = { 0, 0 };
        switch (dir)
        {
        case 0: direction = { -baseSpeed, 0 }; look = Look::LEFT; break;
        case 1: direction = { baseSpeed, 0 }; look = Look::RIGHT; break;
        case 2: direction = { 0, -baseSpeed }; break;
        case 3: direction = { 0, baseSpeed }; break;
        }

        AABB projected = GetHitbox();
        projected.pos.x += direction.x;
        projected.pos.y += direction.y;

        bool canMove = true;
        if (direction.x < 0)
            canMove = !map->TestCollisionWallLeft(projected);
        else if (direction.x > 0)
            canMove = !map->TestCollisionWallRight(projected);

        if (canMove && direction.y != 0)
        {
            if (direction.y < 0)
                canMove = !map->TestCollisionWallUp(projected);
            else
                canMove = !map->TestCollisionWallDown(projected);
        }

        if (canMove)
        {
            movement = direction;
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::WALKING_LEFT : (int)SNOBEEAnim::WALKING_RIGHT);
        }
        else
        {
            movement = { 0, 0 };
            SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
        }
    }

    AABB nextPosHitbox = GetHitbox();
    nextPosHitbox.pos.x += movement.x;
    nextPosHitbox.pos.y += movement.y;

    if (nextPosHitbox.TestAABB(playerBox))
    {
        movement = { 0, 0 };
        SetAnimation((look == Look::LEFT) ? (int)SNOBEEAnim::IDLE_LEFT : (int)SNOBEEAnim::IDLE_RIGHT);
    }
    else
    {
        pos.x += movement.x;
        pos.y += movement.y;
    }

    sprite->Update();

    return false;
}
