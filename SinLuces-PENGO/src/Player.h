#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	16
#define PLAYER_PHYSICAL_HEIGHT	16

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
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE,
	WALKING_LEFT, WALKING_RIGHT, //Pengo
	WALKING_UP, WALKING_DOWN, //Pengo
	PUSH_DOWN, PUSH_LEFT, PUSH_UP, PUSH_RIGHT, //Pengo
	SHOCK_LEFT, SHOCK_RIGHT, //vikings
	TELEPORT_LEFT, TELEPORT_RIGHT, //vikings
	NUM_ANIMATIONS //vikings
};

class Player: public Entity
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

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	int moving;
	bool isMoving();
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	/*void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();*/
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;

	//Reference to the TileMap object
	//This class does not own the object, it only holds a reference to it
	TileMap *map;

	int score;
};

