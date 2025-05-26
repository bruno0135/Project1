#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "EnemyManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
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
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::YELLOWB] =    { 0, 0, n, n   };
	dict_rect[(int)Tile::YELLOWB_2] =  { n, 0, n, n   };
	dict_rect[(int)Tile::YELLOWB_3] =  { 2*n, 0, n, n };
	dict_rect[(int)Tile::BLUEB] =      { 3*n, 0, n, n };
	dict_rect[(int)Tile::ICEBREAK_2] = { 4*n, 0, n, n };
	dict_rect[(int)Tile::ICEBREAK_1] = { 5*n, 0, n, n };
	dict_rect[(int)Tile::ICEBREAK_3] = { 6*n, 0, n, n };
	dict_rect[(int)Tile::ICEBREAK_4] = { 7*n, 0, n, n };
	dict_rect[(int)Tile::ICEBREAK_5] = { 0, n, n, n   };
	dict_rect[(int)Tile::ICEBREAK_6] = { n, n, n, n   };
	dict_rect[(int)Tile::ICEBREAK_7] = { 2*n, n, n, n };
	dict_rect[(int)Tile::ICEBREAK_8] = { 3*n, n, n, n };
	dict_rect[(int)Tile::LIGHTBLUEB] = { 4*n, n, n, n };
	dict_rect[(int)Tile::ORANGEB] =    { 5*n, n, n, n };
	dict_rect[(int)Tile::REDB] =	   { 6*n, n, n, n };
	dict_rect[(int)Tile::PINKB] =      { 7*n, n, n, n };
	dict_rect[(int)Tile::GREENB] =			{ 0, 2*n, n, n   };
	dict_rect[(int)Tile::DIAMONDBLUE] =     { 7*n, 2*n, n, n };
	dict_rect[(int)Tile::PURPLE_STAR] =		{ n,  5*n, n, n  };
	dict_rect[(int)Tile::YELLOW_STAR] =		{ 2*n, 5*n, n, n };
	dict_rect[(int)Tile::LIGHTYELLOW_STAR]= { 3*n, 5*n, n, n };
	dict_rect[(int)Tile::ORANGE_STAR] =		{ 4*n, 5*n, n, n };
	dict_rect[(int)Tile::PINK_STAR] =		{ 5*n, 5*n, n, n };
	dict_rect[(int)Tile::GREEN_STAR] =		{ 6*n, 5*n, n, n };
	dict_rect[(int)Tile::LIGHTBLUE_STAR] =  { 7*n, 5*n, n, n };
	

	dict_rect[(int)Tile::LOCK_RED] = { 6 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LOCK_YELLOW] = { 7 * n, 2 * n, n, n };

	dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	
	dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/Tileset1.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	for (int i = 0; i < size; ++i)
		map[i] = static_cast<Tile>(data[i]);

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if(IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	if (this == nullptr || map == nullptr)
	{
		LOG("Error: TileMap instance or map is null");
		return Tile::AIR;
	}
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y);
		return Tile::AIR;
	}
	return map[idx];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST;
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::LADDER_TOP_L || tile == Tile::LADDER_TOP_R;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::LADDER_L || tile == Tile::LADDER_R;
}
bool TileMap::IsTileLaser(Tile tile) const
{
	return tile == Tile::LASER || tile == Tile::LASER_L || tile == Tile::LASER_R;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(Point(box.pos.x - 1, box.pos.y), box.height);
}

bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(Point(box.pos.x + box.width, box.pos.y), box.height);
}

bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(Point(box.pos.x, box.pos.y - 1), box.width);
}

bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(Point(box.pos.x, box.pos.y + box.height), box.width);
}


bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x = p.x / TILE_SIZE;
	int y0 = p.y / TILE_SIZE;
	int y1 = (p.y + distance - 1) / TILE_SIZE;

	for (int y = y0; y <= y1; ++y)
	{
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int y = p.y / TILE_SIZE;
	int x0 = p.x / TILE_SIZE;
	int x1 = (p.x + distance - 1) / TILE_SIZE;

	for (int x = x0; x <= x1; ++x)
	{
		Tile tile = GetTileIndex(x, y);
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}

//moure un bloc solid
bool TileMap::MoveSolidBlockInPixels(AABB& box, const Point& new_pixel_pos)
{
	int old_tile_x = box.pos.x / TILE_SIZE;
	int old_tile_y = box.pos.y / TILE_SIZE;

	int new_tile_x = new_pixel_pos.x / TILE_SIZE;
	int new_tile_y = new_pixel_pos.y / TILE_SIZE;

	if (old_tile_x < 0 || old_tile_x >= width || old_tile_y < 0 || old_tile_y >= height)
	{
		LOG("Origin position out of bounds");
		return false;
	}
	if (new_tile_x < 0 || new_tile_x >= width || new_tile_y < 0 || new_tile_y >= height)
	{
		LOG("Destination position out of bounds");
		return false;
	}

	Tile old_tile = GetTileIndex(old_tile_x, old_tile_y);
	if (!IsTileSolid(old_tile))
	{
		LOG("No solid tile at origin position");
		return false;
	}

	Tile new_tile = GetTileIndex(new_tile_x, new_tile_y);
	if (IsTileSolid(new_tile))
	{
		LOG("Destination tile is solid, cannot move");
		return false;
	}

	map[new_tile_y * width + new_tile_x] = old_tile;
	map[old_tile_y * width + old_tile_x] = Tile::AIR;

	box.pos = Point(new_tile_x * TILE_SIZE, new_tile_y * TILE_SIZE);

	return true;
}
bool TileMap::TryPushBlock(AABB blockBox, int directionX, int directionY, EnemyManager* enemyManager)
{
	bool pushed = false;
	std::vector<Point> path;

	while (true) {
		int blockX = blockBox.pos.x / TILE_SIZE;
		int blockY = blockBox.pos.y / TILE_SIZE;
		int nextBlockX = blockX + directionX;
		int nextBlockY = blockY + directionY;

		if (nextBlockX < 0 || nextBlockX >= width || nextBlockY < 0 || nextBlockY >= height) {
			break;
		}

		if (IsTileSolid(GetTileIndex(nextBlockX, nextBlockY))) {
			break;
		}

		// Calculem la nova posició
		Point nextPos(nextBlockX * TILE_SIZE, nextBlockY * TILE_SIZE);
		AABB nextBox(nextPos, blockBox.width, blockBox.height);

		// Comprovem si esclafa algun enemic abans de moure el bloc
		if (enemyManager != nullptr) {
			enemyManager->CheckBlockCrush(nextBox); // Aquesta funció ha de fer la comprovació i eliminar si cal
		}

		// Intentem moure el bloc
		if (!MoveSolidBlockInPixels(blockBox, nextPos)) {
			break;
		}

		pushed = true;
		path.push_back(nextPos);

		// Actualitzem la posició del bloc per a la següent iteració
		blockBox.MoveTo(nextPos.x, nextPos.y);
	}

	return pushed;
}


bool TileMap::TestOnLadder(const AABB & box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;
	
	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width-1;
	bottom = box.pos.y + box.height-1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadder(tile1) && IsTileLadder(tile2))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width/2;
		return true;
	}
	return false;
}

bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadderTop(tile1) && IsTileLadderTop(tile2))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;
	
	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::LADDER_L || tile == Tile::LADDER_TOP_L)		return tx * TILE_SIZE + TILE_SIZE;
	else if (tile == Tile::LADDER_R || tile == Tile::LADDER_TOP_R)	return tx * TILE_SIZE;
	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}

bool TileMap::CheckDiamondLines() const
{
	const int needed = 3;

	// Comprova files
	for (int y = 0; y < height; ++y)
	{
		int count = 0;
		for (int x = 0; x < width; ++x)
		{
			Tile tile = GetTileIndex(x, y);
			if (tile == Tile::DIAMONDBLUE) {
				count++;
				LOG("Diamond at (%d,%d), count = %d", x, y, count);
				if (count >= needed) {
					LOG("Found diamond line horizontally at row %d", y);
					return true;
				}
			}
			else {
				if (count > 0)
					LOG("Diamond line broken at (%d,%d), count reset", x, y);
				count = 0;
			}
		}
	}

	// Comprova columnes
	for (int x = 0; x < width; ++x)
	{
		int count = 0;
		for (int y = 0; y < height; ++y)
		{
			Tile tile = GetTileIndex(x, y);
			if (tile == Tile::DIAMONDBLUE) {
				count++;
				LOG("Diamond at (%d,%d), count = %d", x, y, count);
				if (count >= needed) {
					LOG("Found diamond line vertically at column %d", x);
					return true;
				}
			}
			else {
				if (count > 0)
					LOG("Diamond line broken at (%d,%d), count reset", x, y);
				count = 0;
			}
		}
	}

	return false;
}


AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;
	
	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if(!collision) x--;
	}
	box.pos.x = (x+1)*TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x*TILE_SIZE - box.pos.x;
	
	return box;
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
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}
bool TileMap::BreakBlockAt(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height) {
		LOG("Block position (%d,%d) out of bounds", x, y);
		return false;
	}

	int index = y * width + x;
	Tile tile = map[index];
	if (!IsTileSolid(tile)) {
		LOG("Tile at (%d,%d) is not breakable", x, y);
		return false;
	}

	map[index] = Tile::AIR;
	LOG("Block at (%d,%d) broken!", x, y);
	return true;
}
