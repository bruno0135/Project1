
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
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
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/pinguinos/pinguino 1.png") != AppStatus::OK)
	{
		LOG("Failed to load player sprite texture");
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 7 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 3 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_UP, { 5 * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_DOWN, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALK_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALK_RIGHT, { (float)6 * n + (i * n), 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALK_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALK_LEFT, { (float)2 * n + (i * n), 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALK_DOWN, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALK_DOWN, { (float)0 * n + (i * n), 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALK_UP, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALK_UP, { (float)4 * n + (i * n), 0, n, n });


	sprite->SetAnimationDelay((int)PlayerAnim::PUSH_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSH_RIGHT, { (float)6 * n + (i * n), n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::PUSH_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSH_LEFT, { (float)2 * n + (i * n), n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::PUSH_DOWN, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSH_DOWN, { (float)0 * n + (i * n), n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::PUSH_UP, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::PUSH_UP, { (float)4 * n + (i * n), n, n, n });


	sprite->SetAnimation((int)PlayerAnim::IDLE_DOWN);


	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}


//void Player::FreezeAnimationFrame() {
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	int currentAnim = sprite->GetAnimation();
//
//	// Almacenar el delay original solo si no se ha guardado previamente
//	if (originalAnimationDelays.find(currentAnim) == originalAnimationDelays.end()) {
//		originalAnimationDelays[currentAnim] = sprite->GetAnimationDelay(currentAnim);
//	}
//
//	// Establecer un delay muy alto para "congelar" la animaci�n
//	sprite->SetAnimationDelay(currentAnim, 20);
//}

// Funci�n para restaurar el delay original de la animaci�n actual
void Player::RestoreAnimationFrame() {
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int currentAnim = sprite->GetAnimation();

	// Restaurar el delay original si existe
	if (originalAnimationDelays.find(currentAnim) != originalAnimationDelays.end()) {
		sprite->SetAnimationDelay(currentAnim, originalAnimationDelays[currentAnim]);
	}
}

// Funci�n Stop que congela la animaci�n sin cambiar a la animaci�n idle
void Player::Stop() {
	//dir = { 0, 0 };
	//state = State::IDLE;

	//// En lugar de congelar la animaci�n, cambiar a la animaci�n IDLE correspondiente
	//switch (look) {
	//case Look::LEFT:
	//	SetAnimation((int)PlayerAnim::IDLE_LEFT);
	//	break;
	//case Look::RIGHT:
	//	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	//	break;
	//case Look::UP:
	//	SetAnimation((int)PlayerAnim::IDLE_UP);
	//	break;
	//case Look::DOWN:
	//	SetAnimation((int)PlayerAnim::IDLE_DOWN);
	//	break;
	//}
	dir = { 0, 0 }; // Detener movimiento

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite != nullptr)
	{
		if (look == Look::LEFT)
			sprite->SetAnimation((int)PlayerAnim::IDLE_LEFT);
		else if (look == Look::RIGHT)
			sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);
		else if (look == Look::UP)
			sprite->SetAnimation((int)PlayerAnim::IDLE_UP);
		else if (look == Look::DOWN)
			sprite->SetAnimation((int)PlayerAnim::IDLE_DOWN);

		sprite->FreezeAnimationFrame(); // Detener animaci�n
	}
}

// Ejemplo de funci�n para reanudar el movimiento
void Player::ResumeMovement() {
	RestoreAnimationFrame();
	// Aqu� puedes definir la animaci�n de movimiento que corresponda, por ejemplo:
	SetAnimation((int)PlayerAnim::WALK_RIGHT);
	// Se deben actualizar posici�n, direcci�n, etc.
}




void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite && sprite->GetAnimation() != (int)PlayerAnim::WALK_LEFT)
		SetAnimation((int)PlayerAnim::WALK_LEFT);

}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite && sprite->GetAnimation() != (int)PlayerAnim::WALK_RIGHT)
		SetAnimation((int)PlayerAnim::WALK_RIGHT);
}
void Player::StartWalkingDown()
{
	state = State::WALKING;
	look = Look::DOWN;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite && sprite->GetAnimation() != (int)PlayerAnim::WALK_DOWN)
		SetAnimation((int)PlayerAnim::WALK_DOWN);
}
void Player::StartWalkingUp()
{
	state = State::WALKING;
	look = Look::UP;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (sprite && sprite->GetAnimation() != (int)PlayerAnim::WALK_UP)
		SetAnimation((int)PlayerAnim::WALK_UP);

}

//Cambio de animaciones 
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALK_RIGHT); break;
	case State::PUSH: SetAnimation((int)PlayerAnim::PUSH_RIGHT); break;

	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALK_LEFT); break;
	case State::PUSH: SetAnimation((int)PlayerAnim::PUSH_LEFT); break;

	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_UP);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALK_UP); break;
	case State::PUSH: SetAnimation((int)PlayerAnim::PUSH_UP); break;

	}
}
void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_DOWN);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALK_DOWN); break;
	case State::PUSH: SetAnimation((int)PlayerAnim::PUSH_DOWN); break;

	}
}


void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	Move();


	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::Move()
{
	AABB box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	// Actualitza currentDirection si no hi ha direcci� activa
	if (currentDirection == PlayerAnim::NONE) {
		if (IsKeyPressed(KEY_LEFT)) currentDirection = PlayerAnim::WALK_LEFT;
		else if (IsKeyPressed(KEY_RIGHT)) currentDirection = PlayerAnim::WALK_RIGHT;
		else if (IsKeyPressed(KEY_UP)) currentDirection = PlayerAnim::WALK_UP;
		else if (IsKeyPressed(KEY_DOWN)) currentDirection = PlayerAnim::WALK_DOWN;
	}

	// Si la tecla activa s'ha deixat de pr�mer, netegem la direcci� activa
	switch (currentDirection)
	{
	case PlayerAnim::WALK_LEFT:
		if (!IsKeyDown(KEY_LEFT)) currentDirection = PlayerAnim::NONE;
		break;
	case PlayerAnim::WALK_RIGHT:
		if (!IsKeyDown(KEY_RIGHT)) currentDirection = PlayerAnim::NONE;
		break;
	case PlayerAnim::WALK_UP:
		if (!IsKeyDown(KEY_UP)) currentDirection = PlayerAnim::NONE;
		break;
	case PlayerAnim::WALK_DOWN:
		if (!IsKeyDown(KEY_DOWN)) currentDirection = PlayerAnim::NONE;
		break;
	default: break;
	}

	// Executa moviment segons la direcci� activa
	switch (currentDirection)
	{
	case PlayerAnim::WALK_LEFT:
		pos.x -= PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else if (!IsLookingLeft()) ChangeAnimLeft();
		box = GetHitbox();
		if (map->TestCollisionWallLeft(box)) {
			pos.x = prev_x;
			if (state == State::IDLE) Stop();
		}
		break;

	case PlayerAnim::WALK_RIGHT:
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else if (!IsLookingRight()) ChangeAnimRight();
		box = GetHitbox();
		if (map->TestCollisionWallRight(box)) {
			pos.x = prev_x;
			if (state == State::IDLE) Stop();
		}
		break;

	case PlayerAnim::WALK_UP:
		pos.y -= PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingUp();
		else if (!IsLookingUp()) ChangeAnimUp();
		box = GetHitbox();
		if (map->TestCollisionWallLeft(box)) {  // potser hauria de ser WallTop
			pos.y = prev_y;
			if (state == State::PUSH) Stop();
		}
		break;

	case PlayerAnim::WALK_DOWN:
		pos.y += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingDown();
		else if (!IsLookingDown()) ChangeAnimDown();
		box = GetHitbox();
		if (map->TestCollisionWallRight(box)) {  // potser hauria de ser WallBottom
			pos.y = prev_y;
			if (state == State::IDLE) Stop();
		}
		break;

	default:
		break;
	}

	// Comprovaci� de col�lisions amb el terra
	box = GetHitbox();
	if (map->TestCollisionGround(box, &pos.y)) {
		if (state == State::PUSH) Stop();
	}

	// Si no es prem cap tecla, atura el jugador
	if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN))
	{
		currentDirection = PlayerAnim::NONE;

		Sprite* sprite = dynamic_cast<Sprite*>(render);
		bool isIdleAnimation = false;

		if (look == Look::LEFT && sprite->GetAnimation() == (int)PlayerAnim::IDLE_LEFT)
			isIdleAnimation = true;
		else if (look == Look::RIGHT && sprite->GetAnimation() == (int)PlayerAnim::IDLE_RIGHT)
			isIdleAnimation = true;
		else if (look == Look::UP && sprite->GetAnimation() == (int)PlayerAnim::IDLE_UP)
			isIdleAnimation = true;
		else if (look == Look::DOWN && sprite->GetAnimation() == (int)PlayerAnim::IDLE_DOWN)
			isIdleAnimation = true;

		if (state != State::IDLE || !isIdleAnimation)
			Stop();
	}
}



void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}