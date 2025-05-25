#pragma once
#include "Entity.h"
#include "TileMap.h"
#include <unordered_map>

// Forward declaration d'EnemyManager per evitar incloure tot el seu header
class EnemyManager;

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	12

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1
#define PLAYER_SPEEDY			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		10

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, PUSH, DESTROYING, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
	WALK_LEFT, WALK_RIGHT, WALK_DOWN, WALK_UP,
	PUSH_LEFT, PUSH_RIGHT, PUSH_DOWN, PUSH_UP,
	NUM_ANIMATIONS, NONE
};

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	void TakeDamage(int amount);
	bool CanTakeDamage() const;
	void StartDamageCooldown();
	void UpdateDamageCooldown(float deltaTime);
	int GetHealth() const;
	void SetHealth(int newHealth) { health = newHealth; }
	void SetEnemyManager(EnemyManager* manager);

	bool HasWon() const { return hasWon; }
	AABB GetHitbox() const;

	void RestoreAnimationFrame();

	void Stop();
	void ResumeMovement();

private:

	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingDown() const;
	bool IsLookingUp() const;

	//Player mechanics
	void Move();
	PlayerAnim currentDirection = PlayerAnim::NONE;

	// Funció per obtenir la posició del tile davant segons direcció
	Point GetFrontTilePos(int dx, int dy) const;

	//Animations coordination
	std::unordered_map<int, int> originalAnimationDelays;
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();

	//Funció per cridar quan es guanya
	void OnPlayerWin();

	void TryDestroyBlock();


	EnemyManager* enemyManager = nullptr;
	State state;
	Look look;
	TileMap* map;
	int score;
	int health = 4; // Aqui se cambian las vidas 
	float damageCooldownTimer = 0.0f;
	float damageCooldownTime = 1.0f; // 1 segon de cooldown
	bool isDamageCooldownActive = false;

	bool isDestroying = false;
	Point blockToDestroyTile;
	int destroyFrameCounter = 0;
	const int destroyAnimDurationFrames = 48;

	// indica que el jugador ha guanyat
	bool hasWon = false;
};
