
#include "Entity.h"
#include "Sprite.h"
#include <cmath>
#include "Globals.h"

Entity::Entity() :
	pos({0,0}), dir({0,0}), width(0), height(0), frame_width(0), frame_height(0), render(nullptr)
{
	alive = false;
}
Entity::Entity(const Point& p, int w, int h) :
	pos(p), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
	alive = true;
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) :
	pos(p), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
	alive = true;
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
void Entity::Set(const Point& p, const Point& d, int w, int h, int framew, int frameh)
{
	pos = p;
	dir = d;
	width = w;
	height = h;
	frame_width = framew;
	frame_height = frameh;
	alive = true;
}
void Entity::Update()
{
	pos += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void Entity::SetAlive(bool b)
{
	alive = b;
}
bool Entity::IsAlive() const
{
	return alive;
}
Point Entity::GetRenderingPosition() const
{
	Point p;
	p.x = pos.x + width / 2 - frame_width / 2;
	p.y = pos.y - (frame_height-1);
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

	render->DrawBox(pos.x, pos.y-(height-1), width, height, c);
	render->DrawCorners(pos.x, pos.y-(height-1), width, height);
}
void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y-(h-1), w, h, c);
	render->DrawCorners(x, y-(h-1), w, h);
}

void Entity::SetAnimation(int anim_id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite) sprite->SetAnimation(anim_id);
}
