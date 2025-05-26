#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>


class EnemyManager;

enum class Tile {

	// -1: area covered by entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLUEB = 1, YELLOWB_2, YELLOWB_3, YELLOWB,
	ICEBREAK_1, ICEBREAK_2, ICEBREAK_3, ICEBREAK_4, ICEBREAK_5, ICEBREAK_6, ICEBREAK_7, ICEBREAK_8,
	LIGHTBLUEB, ORANGEB, REDB, PINKB, GREENB, DIAMONDBLUE,
	BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	BLOCK_BEAM_L, BLOCK_BEAM_R,

	LADDER_L = 20, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
	LOCK_RED = 30, LOCK_YELLOW,
	LASER_L = 40, LASER_R,

	// 50 <= id < 100: special tiles
	DOOR = 50, PURPLE_STAR, YELLOW_STAR, LIGHTYELLOW_STAR, ORANGE_STAR, PINK_STAR, GREEN_STAR, LIGHTBLUE_STAR,
	LASER = 70, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,

	// id >= 100: entities' initial locations
	PLAYER = 100,
	SNOBEE = 200,
	TURRET_LEFT = 301, TURRET_RIGHT = 302,

	//Intervals
	SOLID_FIRST = BLUEB,
	SOLID_LAST = BLOCK_BEAM_R,
	OBJECT_FIRST = PURPLE_STAR,
	OBJECT_LAST = LIGHTBLUE_STAR,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = TURRET_RIGHT
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update(float deltaTime);
	void Render();
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	bool TestCollisionWallUp(const AABB& box) const;
	bool TestCollisionWallDown(const AABB& box) const;

	bool TestCollisionGround(const AABB& box, int* py) const;
	bool TestFalling(const AABB& box) const;

	bool TestOnLadder(const AABB& box, int* px) const;
	bool TestOnLadderTop(const AABB& box, int* px) const;

	AABB GetSweptAreaX(const AABB& hitboxbox) const;
	bool MoveSolidBlockInPixels(AABB& box, const Point& new_pixel_pos);
	bool TryPushBlock(AABB blockBox, int directionX, int directionY, EnemyManager* enemyManager);
	bool CheckDiamondLines() const;
	bool BreakBlockAt(int x, int y);

	Tile GetTile(int x, int y) const;// retorna l'ID del tile a la posició
void SetTile(int x, int y, Tile tile);

	// ðŸ”§ NUEVAS FUNCIONES PARA MOVIMIENTO DE BLOQUES
	bool IsBlockAt(int x, int y) const;
	bool IsWallAt(int x, int y) const;
	void RemoveBlock(int x, int y);
	void MoveBlock(int fromX, int fromY, int toX, int toY);

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool IsTileLaser(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite* laser;

	int Width, Height;
	std::vector<Tile> tiles;
	//Tile sheet
	const Texture2D* img_tiles;
};
