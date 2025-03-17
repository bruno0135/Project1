#include "Inky.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Inky::Inky(const Point& p, GhostState s, Directions d) : Ghost(p, s, d)
{
	type = GhostType::INKY;
	map = nullptr;
	score = 0;
}

void Inky::Update()
{
	UpdateStates();

	if (state == GhostState::SCATTLE)
	{
		GoPath({ INKY_SCATTER_X, INKY_SCATTER_Y });
	}
	else if (state == GhostState::CHASE)
	{
		Point frontPlayerPos = player->GetCenterPosition();
		switch (player->GetDirection())
		{
		case Directions::UP: frontPlayerPos.y -= (PATH_SIZE * 2);
			frontPlayerPos.x -= (PATH_SIZE * 2);
			break;
		case Directions::DOWN: frontPlayerPos.y += (PATH_SIZE * 2);
			break;
		case Directions::RIGHT: frontPlayerPos.x += (PATH_SIZE * 2);
			break;
		case Directions::LEFT:frontPlayerPos.x -= (PATH_SIZE * 2);
			break;
		default:
			break;
		}

		Point blinkyPos = blinky->GetCenterPosition();

		Point posToGo = { 2 * frontPlayerPos.x - blinkyPos.x, 2 * frontPlayerPos.y - blinkyPos.y };

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

void Inky::SetBlinky(Blinky* p)
{
	blinky = p;
}