
#include "Entity.h"
#include <cmath>
#include "Globals.h"

Entity::Entity(const Point& p, int w, int h) : pos(p), hitBoxOffset({ 0,0 }), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
}
Entity::Entity(const Point& p, const Point& ho, int w, int h) : pos(p), hitBoxOffset(ho), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) : pos(p), hitBoxOffset({ 0,0 }), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
}
Entity::Entity(const Point& p, const Point& ho, int w, int h, int frame_w, int frame_h) : pos(p), hitBoxOffset(ho), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}
void Entity::SetPos(const Point& p)
{
	pos = p;
}
void Entity::Update()
{
	pos += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(pos.x+hitBoxOffset.x, pos.y+hitBoxOffset.y);
	AABB hitbox(p, width, height);
	return hitbox;
}
Point Entity::GetCenterPosition()
{
	Point p(pos.x + width / 2, pos.y + height / 2);
	return p;
}Point Entity::GetPosition()
{
	return pos;
}
Point Entity::GetRenderingPosition() const
{
	Point p(pos.x,pos.y);
	return p;
}
void Entity::Draw() const
{
	Point p = GetRenderingPosition();
	render->Draw(p.x, p.y);
}
void Entity::DrawTint(const Color& col) const
{
	Point p = GetRenderingPosition();
	render->DrawTint(p.x, p.y, col);
}
void Entity::DrawHitbox(const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	Point p(pos.x + hitBoxOffset.x, pos.y + hitBoxOffset.y);
	render->DrawBox(p.x, p.y, width, height, c);
	render->DrawCorners(p.x, p.y, width, height);
}
void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y, w, h, c);
	render->DrawCorners(x, y, w, h);
}

int Entity::CalculateDistanceWithCenter(Point p)
{
	int dx = GetCenterPosition().x - p.x;
	int dy = GetCenterPosition().y - p.y;
	return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

int Entity::CalculateDistance(Point p)
{
	int dx = pos.x - p.x;
	int dy = pos.y - p.y;
	return static_cast<float>(std::sqrt(dx * dx + dy * dy));
}
