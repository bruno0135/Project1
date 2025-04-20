#pragma once
#include "Entity.h"
#include "TileMap.h"



//Representation model size: 32x32
#define SNOBEE_FRAME_SIZE		16
//Logical model size: 24x30
#define SNOBEE_PHYSICAL_WIDTH	16
#define SNOBEE_PHYSICAL_HEIGHT	16

//Representation model size: 32x32
#define TURRET_FRAME_SIZE		32
//Logical model size: 0x0
#define TURRET_PHYSICAL_WIDTH	32
#define TURRET_PHYSICAL_HEIGHT	26

enum class EnemyType { SNOBEE, TURRET };

class Enemy : public Entity
{
public:
	Enemy(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Enemy();

	//Draw the maximum visibility area of the enemy
	void DrawVisibilityArea(const Color& col) const;

	virtual AppStatus Initialise(const Point& pos, EnemyType type, const AABB& area, TileMap* map) = 0;



	//Update the enemy according to its logic, return true if the enemy must shoot
	virtual bool Update(const AABB& box) = 0;

	//Retrieve the position and direction of the shot to be thrown


protected:
	//Return true if the given hitbox is within the visibility area and the enemy is facing it
	bool IsVisible(const AABB& hitbox);

	Look look;
	AABB visibility_area;
};

