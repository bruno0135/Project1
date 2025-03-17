#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"
#include "Player.h"
#include "Blinky.h"

#define INKY_SCATTER_X 472
#define INKY_SCATTER_Y 472

class Inky : public Ghost
{
	public:
		Inky(const Point& p, GhostState s, Directions d);
		void Update() override;
		void SetBlinky(Blinky* player);

	private:
		Blinky* blinky;
};