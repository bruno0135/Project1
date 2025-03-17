#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "LogMessages.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_tiles[(int)Tile::EMPTY] = { n, n, n, n };

	dict_tiles[(int)Tile::BLACK_BLOCK] = { n, n, n, n };

	dict_tiles[(int)Tile::WALL_OUTSIDE_UP] = { n,  0, n, n };

	dict_tiles[(int)Tile::WALL_OUTSIDE_DOWN] = { n,  n * 3, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_LEFT] = { 0, n, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_RIGHT] = { n * 3, n, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_C_UP_LEFT] = { 0,  0, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_C_UP_RIGHT] = { 3 * n,  0, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_C_DOWN_LEFT] = { 0, n * 3, n, n };
	dict_tiles[(int)Tile::WALL_OUTSIDE_C_DOWN_RIGHT] = { n * 3, n * 3, n, n };

	dict_tiles[(int)Tile::WALL_INSIDE_C_UP_LEFT] = { 5 * n,  n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_C_UP_RIGHT] = { 6 * n,  n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_C_DOWN_LEFT] = { 5 * n, 2 * n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_UP_LEFT] = { 5 * n, 0, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_UP_RIGHT] = { 6 * n, 0, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_RIGHT_UP] = { 7 * n, n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_RIGHT_DOWN] = { 7 * n, 2 * n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_DOWN_RIGHT] = { 6 * n, 3 * n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_DOWN_LEFT] = { 5 * n, 3 * n, n, n };	
	dict_tiles[(int)Tile::WALL_INSIDE_LEFT_DOWN] = { 4 * n, 2 * n, n, n };
	dict_tiles[(int)Tile::WALL_INSIDE_LEFT_UP] = { 4 * n, 1 * n, n, n };

	dict_tiles[(int)Tile::DOBLE_RECT_LEFT] = { 12 * n, 1 * n, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_C_UP_LEFT] = { 12 * n, 0, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_C_DOWN_LEFT] = { 12 * n, n*3, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_UP] = { 13 * n, 0, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_C_UP_RIGHT] = { 15 * n, 0, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_RIGHT] = { 15 * n, n, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_C_DOWN_RIGHT] = { 15 * n, 3 * n, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_DOWN] = { 13 * n, 3 * n, n, n };
	dict_tiles[(int)Tile::DOBLE_RECT_C_DOWN_LEFT] = { 12 * n, n * 3, n, n };
	
	dict_tiles[(int)Tile::DOBLE_CURVED_C_UP_LEFT] = { 8 * n, 0, n, n };
	dict_tiles[(int)Tile::DOBLE_CURVED_C_UP_RIGHT] = { 11 * n, 0, n, n };
	dict_tiles[(int)Tile::DOBLE_CURVED_C_DOWN_LEFT] = { 8 * n, n * 3, n, n };
	dict_tiles[(int)Tile::DOBLE_CURVED_C_DOWN_RIGHT] = { 11 * n, n * 3, n, n };
	
	dict_tiles[(int)Tile::DOOR_CORNER_LEFT] = { 13 * n, n * 1, n, n };
	dict_tiles[(int)Tile::DOOR_CORNER_RIGHT] = { 14 * n, n * 1, n, n };

	dict_tiles[(int)Tile::DOOR] = { 16 * n, 0, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(ResourceType::IMG_MAP, "resources/sprites/MapX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	if (data.LoadTexture(ResourceType::IMG_MAP_WHITE, "resources/sprites/MapWhiteX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(ResourceType::IMG_MAP);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	
}

Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(box.pos , box.height);
}
bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(box.pos + Point(0, box.height-1), box.height);
}

bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
int TileMap::GetTileCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;

	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	return 0;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR && tile!=Tile::EMPTY && tile != Tile::TP_LEFT && tile != Tile::TP_RIGHT)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

					rc = dict_tiles[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
			}
		}
	}
}
void TileMap::RenderEmptys()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if ( tile == Tile::EMPTY || tile == Tile::TP_LEFT || tile == Tile::TP_RIGHT)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				rc = dict_tiles[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);
			}
		}
	}
}

void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_MAP);

	dict_tiles.clear();
}

void TileMap::ChangeMap(bool white)
{
	ResourceManager& data = ResourceManager::Instance();
	if (white) img_tiles = data.GetTexture(ResourceType::IMG_MAP_WHITE);
	else img_tiles = data.GetTexture(ResourceType::IMG_MAP);
}