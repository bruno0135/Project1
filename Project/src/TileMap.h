#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {
	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLACK_BLOCK = 1, WALL_OUTSIDE_UP, WALL_OUTSIDE_DOWN, WALL_OUTSIDE_RIGHT, WALL_OUTSIDE_LEFT, WALL_OUTSIDE_C_UP_RIGHT, WALL_OUTSIDE_C_UP_LEFT, WALL_OUTSIDE_C_DOWN_RIGHT, WALL_OUTSIDE_C_DOWN_LEFT,
	WALL_INSIDE_LEFT_UP, WALL_INSIDE_C_UP_LEFT, WALL_INSIDE_UP_LEFT, WALL_INSIDE_UP_RIGHT, WALL_INSIDE_C_UP_RIGHT, WALL_INSIDE_RIGHT_UP, WALL_INSIDE_RIGHT_DOWN, WALL_INSIDE_C_DOWN_RIGHT, WALL_INSIDE_DOWN_RIGHT, WALL_INSIDE_DOWN_LEFT, WALL_INSIDE_C_DOWN_LEFT, WALL_INSIDE_LEFT_DOWN,
	DOBLE_RECT_LEFT, DOBLE_RECT_C_UP_LEFT, DOBLE_RECT_UP, DOBLE_RECT_C_UP_RIGHT, DOBLE_RECT_RIGHT, DOBLE_RECT_C_DOWN_RIGHT, DOBLE_RECT_DOWN, DOBLE_RECT_C_DOWN_LEFT,
	DOBLE_CURVED_C_UP_LEFT, DOBLE_CURVED_C_UP_RIGHT, DOBLE_CURVED_C_DOWN_LEFT, DOBLE_CURVED_C_DOWN_RIGHT, BLACK_WITH_COLLISIONS,
	DOOR_CORNER_LEFT, DOOR_CORNER_RIGHT, 

	// 50 <= id < 100: special tiles
	DOOR = 50, SMALL_PELET, LARGE_PELET, TP_LEFT, TP_RIGHT,

	// id >= 100: entities' initial locations
	PLAYER = 100, GHOST, FRUIT,

	//Intervals
	STATIC_FIRST = BLACK_BLOCK,
	STATIC_LAST = DOOR,
	SOLID_FIRST = BLACK_BLOCK,
	SOLID_LAST = DOOR,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = DOOR,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = FRUIT
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void RenderEmptys();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const; 
	void ChangeMap(bool white);

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	
	bool IsTileSolid(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetTileCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_tiles;

	//Tile sheet
	const Texture2D* img_tiles;
};

