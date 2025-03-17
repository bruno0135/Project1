#include "Clyde.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Clyde::Clyde(const Point& p, GhostState s, Directions d) : Ghost(p, s, d)
{
	type = GhostType::CLYDE;
	map = nullptr;
	score = 0;
}


void Clyde::Update()
{
	UpdateStates();

	if (state == GhostState::SCATTLE) 
	{
		GoPath({ CLYDE_SCATTER_X, CLYDE_SCATTER_Y });
	}
	else if (state == GhostState::CHASE)
	{
		if (CalculateDistanceWithCenter(player->GetCenterPosition()) > CLYDE_MAX_DIST) GoPath(player->GetCenterPosition());
		else GoPath({ CLYDE_SCATTER_X, CLYDE_SCATTER_Y });
	}
	else if (state == GhostState::FRIGHTENED)
	{
		MoveRandomly();
	}
	else if (state == GhostState::EATEN)
	{
		if (navMesh->CheckItsJustAbove(pos, GHOST_FRAME_SIZE))
		{
			speed = GHOST_SPEED * 2;
		}
		if (navMesh->RightHalf(navMesh->GetPathIndex(GetCenterPosition().x, GetCenterPosition().y))) GoPath({ RIGHT_SPAWN_POS_X, SPAWN_POS_Y });
		else GoPath({ LEFT_SPAWN_POS_X, SPAWN_POS_Y });
	}

	Move();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}