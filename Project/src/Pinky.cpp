#include "Pinky.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"


Pinky::Pinky(const Point& p, GhostState s, Directions d) : Ghost(p, s, d)
{
	type = GhostType::PINKY;
	map = nullptr;
	score = 0;
}

void Pinky::Update()
{
	UpdateStates();
	
	if (state == GhostState::SCATTLE) //CHASE
	{
		GoPath({ PINKY_SCATTER_X, PINKY_SCATTER_Y });
	}
	else if (state == GhostState::CHASE)
	{
		Point posToGo = player->GetCenterPosition();
		switch (player->GetDirection())
		{
		case Directions::UP: posToGo.y -= (PATH_SIZE * 4);
			posToGo.x -= (PATH_SIZE * 4);
			break;
		case Directions::DOWN: posToGo.y += (PATH_SIZE * 4);
			break;
		case Directions::RIGHT: posToGo.x += (PATH_SIZE * 4);
			break;
		case Directions::LEFT:posToGo.x -= (PATH_SIZE * 4);
			break;
		default:
			break;
		}

		GoPath(posToGo);
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