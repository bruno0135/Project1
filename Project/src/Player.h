#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	32
#define PLAYER_PHYSICAL_HEIGHT	32

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			2

//Logic states
enum class PlayerState { IDLE, WALKING, DEAD };

//Rendering states
enum class PlayerAnim {
	BITE_UP, BITE_DOWN,
	BITE_LEFT, BITE_RIGHT,
	DIE, NUM_ANIMATIONS
};

class Player : public Entity
{
public:
	Player(const Point& p, PlayerState s, Directions dir);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore(int n);
	void InitLives(int n);
	void IncrScore(int n); 
	void IncrLives(int n); 
	void DecrLives(int n);
	int GetScore();
	int GetLives();
	void Die();

	void Update();
	void DrawDebug(const Color& col);
	void Release();
	void Reload();
	Directions GetDirection();
	void SetAnimation(int id, bool loop = true);

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;
	bool JustOneKeyIsDown();

	//Player mechanics
	void Move();
	void ChangeMove();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingDown();
	void StartWalkingUp();
	void Stop();

	//Animation management
	PlayerAnim GetAnimation();
	void SetCurrentDelayToAnimation(int i);

	PlayerState state;
	Directions direction;

	TileMap* map;

	int score;
	int lives;
	bool died;
};

