#include "Blinky.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "LogMessages.h"


Blinky::Blinky(const Point& p, GhostState s, Directions d) : Ghost(p, s, d)
{
	type = GhostType::BLINKY;
	map = nullptr;
	score = 0;
}

void Blinky::Update()
{
	UpdateStates();

	if (state == GhostState::SCATTLE) 
	{
		GoPath({ BLINKY_SCATTER_X, BLINKY_SCATTER_Y });
	}
	else if (state == GhostState::CHASE)
	{
		GoPath(player->GetCenterPosition());
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