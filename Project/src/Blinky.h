#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"
#include "player.h"

#define BLINKY_SCATTER_X 456
#define BLINKY_SCATTER_Y 24

#pragma once
class Blinky : public Ghost
{
	public:
		Blinky(const Point& p, GhostState s, Directions d);
		void Update() override;

		private:
};

