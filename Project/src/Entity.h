#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"

class Entity
{
public:
	Entity(const Point& p, const Point& hp, int width, int height);
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	Entity(const Point& p, const Point& hp, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();

	void SetPos(const Point& p);
	void Update();
	AABB GetHitbox() const;
	Point GetCenterPosition();
	Point GetPosition();
	int CalculateDistance(Point p);
	int CalculateDistanceWithCenter(Point p);

	//Draw representation model
	void Draw() const;
	void DrawTint(const Color& col) const;

	//Draw logical/physical model (hitbox)
	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

protected:
	Point GetRenderingPosition() const;

	//Logical/physical model
	Point pos, dir, hitBoxOffset;
	int width, height;

	//Representation model
	int frame_width, frame_height;

	RenderComponent* render;
};