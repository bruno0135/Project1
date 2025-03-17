#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "NavMesh.h"
#include "Player.h"
#include <algorithm>
#include <random>
#include <raymath.h>

//Representation model size: 32x32
#define GHOST_FRAME_SIZE		32

//Logical model size: 12x28
#define GHOST_PHYSICAL_WIDTH	32
#define GHOST_PHYSICAL_HEIGHT	32

//Horizontal speed and vertical speed while falling down
#define GHOST_SPEED				2

//Logic states
enum class GhostState { CHASE, SCATTLE, FRIGHTENED, EATEN, DEAD };

enum class GhostType { BLINKY, PINKY, INKY, CLYDE };

//Rendering states
enum class GhostAnim {
	WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT, FRIGHTENED, FRIGHTENED_WHITE, EATEN_UP, EATEN_DOWN, EATEN_LEFT, EATEN_RIGHT,
	NUM_ANIMATIONS
};

class Ghost : public Entity
{
public:
	Ghost(const Point& p, GhostState s, Directions d);
	~Ghost();

	virtual AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);
	void SetNavMesh(NavMesh* navMesh);
	void SetPlayer(Player* player);
	void ChangeCommonState(GhostState state);
	void ChangeState(GhostState state);
	void Reload();
	void StartWalking(Directions dir);

	virtual void Update();
	virtual void UpdateStates();
	GhostState GetState();
	void DrawDebug(const Color& col);
	void DrawDebugNavmesh(const Color& col);
	void DrawPathBetweenPoints(Point start, Point end, const Color& col);
	void Release();

protected:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	//mechanics
	void Move();
	void GoPath(Point point);
	void MoveRandomly();
	bool ChangeRoute(Point point);
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingDown();
	void StartWalkingUp();
	
	Directions OppositeDirection(Directions dir);
	std::vector<Directions> GetDirectionsCanMove();

	//Animation management
	void SetAnimation(int id);
	GhostAnim GetAnimation();

	GhostType type;
	GhostState state;
	GhostState commonState;
	Directions direction;
	int speed;

	TileMap* map;
	NavMesh* navMesh;
	Player* player;
	std::vector<Movement> route;

	int score;
	float stateChangeTime;
};
