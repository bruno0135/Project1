#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Player::Player(const Point& p, PlayerState s, Directions dir) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	direction = dir;
	map = nullptr;
	score = 0;
}
Player::~Player()
{

}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_PLAYER, "resources/sprites/Pac-manX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::BITE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)2 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)0 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_RIGHT, { (float)1 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)5 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)3 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_LEFT, { (float)4 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)8 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)6 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_UP, { (float)7 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::BITE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)11 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)10 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)9 * n, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::BITE_DOWN, { (float)10 * n, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::DIE, ANIM_DELAY*2);
	for (i = 0; i < 11; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DIE, { (float)i * n, 1 * n, n, n });
	
	Reload();

	return AppStatus::OK;
}
void Player::Reload()
{
	state = PlayerState::WALKING;
	direction = Directions::RIGHT;
	SetAnimation((int)PlayerAnim::BITE_RIGHT); 
	died = false;
}
void Player::InitScore(int n)
{
	score = n;
}
void Player::IncrScore(int n)
{
	score += n;
}
void Player::IncrLives(int n)
{
	lives += n;
}
int Player::GetScore()
{
	return score;
}
int Player::GetLives()
{
	return lives;
}
void Player::InitLives(int n)
{
	lives = n;
}
void Player::DecrLives(int n)
{
	lives -= n;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return direction == Directions::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return direction == Directions::LEFT;
}
bool Player::IsLookingUp() const
{
	return direction == Directions::UP;
}
bool Player::IsLookingDown() const
{
	return direction == Directions::DOWN;
}
bool Player::JustOneKeyIsDown()
{	
	int keysDown = 0;
	
	if (IsKeyDown(KEY_LEFT))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_UP))
	{
		keysDown = keysDown + 1;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		keysDown = keysDown + 1;
	}

	if (keysDown == 1)
		return true;
	else return false;
}
void Player::SetAnimation(int id, bool b)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id, b);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::SetCurrentDelayToAnimation(int i)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetCurrentDelay(i);
}
void Player::Stop()
{
	dir = { 0,0 };
	state = PlayerState::IDLE;
}
void Player::StartWalkingLeft()
{
	state = PlayerState::WALKING;
	direction = Directions::LEFT;
	SetAnimation((int)PlayerAnim::BITE_LEFT);
}
void Player::StartWalkingRight()
{
	state = PlayerState::WALKING;
	direction = Directions::RIGHT;
	SetAnimation((int)PlayerAnim::BITE_RIGHT);
}
void Player::StartWalkingUp()
{
	state = PlayerState::WALKING;
	direction = Directions::UP;
	SetAnimation((int)PlayerAnim::BITE_UP);
}
void Player::StartWalkingDown()
{
	state = PlayerState::WALKING;
	direction = Directions::DOWN;
	SetAnimation((int)PlayerAnim::BITE_DOWN);
}

void Player::Update()
{
	if (!died)
	{
		ChangeMove();
		if (state == PlayerState::WALKING)
		{
			Move();
		}
		else if (state == PlayerState::IDLE)
		{
			SetCurrentDelayToAnimation(1000); //PAUSAR ANIMACIO
		}
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::Die()
{
	died = true;
	SetAnimation((int)PlayerAnim::DIE, false);
}

void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	if (direction == Directions::RIGHT)
	{
		pos.x += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else if (direction == Directions::LEFT)
	{
		pos.x += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else if (direction == Directions::UP)
	{
		pos.y += -PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == PlayerState::WALKING) Stop();
		}
	}
	else
	{
		pos.y += PLAYER_SPEED;

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == PlayerState::WALKING) Stop();
		}
	}
}

void Player::ChangeMove()
{
	if (JustOneKeyIsDown())
	{
		AABB box;
		int prev_x = pos.x;
		int prev_y = pos.y;

		if (IsKeyDown(KEY_LEFT) && direction != Directions::LEFT)
		{
			pos.x += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallLeft(box))
				StartWalkingLeft();
		}
		else if (IsKeyDown(KEY_RIGHT) && direction != Directions::RIGHT)
		{
			pos.x += PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallRight(box)) StartWalkingRight();
		}
		else if (IsKeyDown(KEY_UP) && direction != Directions::UP)
		{
			pos.y += -PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallUp(box)) StartWalkingUp();
		}
		else if (IsKeyDown(KEY_DOWN) && direction != Directions::DOWN)
		{
			pos.y += PLAYER_SPEED;

			box = GetHitbox();
			if (!map->TestCollisionWallDown(box)) StartWalkingDown();
		}
		pos.x = prev_x;
		pos.y = prev_y;
	}
}

Directions Player::GetDirection()
{
	return direction;
}


void Player::DrawDebug(const Color& col)
{
	Entity::DrawHitbox(GREEN);

	DrawText(TextFormat("Player\nPosition: (%d,%d)\nSize: %dx%d\nFrame: %dx%d\n", pos.x, pos.y, width, height, frame_width, frame_height), 0, -100, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE); 
	DrawPixel(GetCenterPosition().x, GetCenterPosition().y, RED);
}

void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_PLAYER);

	render->Release();
}