#pragma once
#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"
#include "Player.h"
#include "Blinky.h"

#define PINKY_SCATTER_X 88
#define PINKY_SCATTER_Y 24

class Pinky : public Ghost
{
	public:
		Pinky(const Point& p, GhostState s, Directions d);
		void Update() override;
	private:
		Blinky* blinky;
};