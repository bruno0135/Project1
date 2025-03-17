#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"
#include "Player.h"

#define CLYDE_SCATTER_X 72
#define CLYDE_SCATTER_Y 472
#define CLYDE_MAX_DIST 128

class Clyde : public Ghost
{
	public:
		Clyde(const Point& p, GhostState s, Directions d);
		void Update() override;

	private:
};