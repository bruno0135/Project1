
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "EnemyManager.h"
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


// Función para restaurar el delay original de la animación actual
void Player::RestoreAnimationFrame() {
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int currentAnim = sprite->GetAnimation();

	// Restaurar el delay original si existe
	if (originalAnimationDelays.find(currentAnim) != originalAnimationDelays.end()) {
		sprite->SetAnimationDelay(currentAnim, originalAnimationDelays[currentAnim]);
	}
}

// Función Stop que congela la animación sin cambiar a la animación idle
void Player::Stop() {
	
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

		sprite->FreezeAnimationFrame(); // Detener animación
	}
}

// Ejemplo de función para reanudar el movimiento
void Player::ResumeMovement() {
	RestoreAnimationFrame();
	// Aquí puedes definir la animación de movimiento que corresponda, por ejemplo:
	SetAnimation((int)PlayerAnim::WALK_RIGHT);
	// Se deben actualizar posición, dirección, etc.
}
void Player::TakeDamage(int amount)
{
	if (!CanTakeDamage())
		return;
	health -= amount;
	if (health < 0) health = 0;

	isDamageCooldownActive = true;
	damageCooldownTimer = 0.0f;
	// Pots afegir aquí efectes com sons, animacions, etc.
}
bool Player::CanTakeDamage() const {
	return !isDamageCooldownActive;
}
void Player::StartDamageCooldown() {
	damageCooldownTimer = damageCooldownTime;
}
void Player::UpdateDamageCooldown(float deltaTime) {
	if (isDamageCooldownActive) {
		damageCooldownTimer += deltaTime;
		if (damageCooldownTimer >= damageCooldownTime) {
			isDamageCooldownActive = false;
			damageCooldownTimer = 0.0f;
		}
	}
}
int Player::GetHealth() const
{
	return health;
}
void Player::SetEnemyManager(EnemyManager* manager) {
	enemyManager = manager;
}
AABB Player::GetHitbox() const {
	int hitboxHeight = height; // alçada de la hitbox
	Point hitboxPos(pos.x, pos.y - hitboxHeight + 1);	
	return AABB(hitboxPos, width, hitboxHeight);
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

void Player::Update()
{
	Move();
	// Escollim animació segons estat i direcció
	switch (state)
	{
	case State::IDLE:
		switch (look)
		{
		case Look::LEFT:   SetAnimation((int)PlayerAnim::IDLE_LEFT); break;
		case Look::RIGHT:  SetAnimation((int)PlayerAnim::IDLE_RIGHT); break;
		case Look::UP:     SetAnimation((int)PlayerAnim::IDLE_UP); break;
		case Look::DOWN:   SetAnimation((int)PlayerAnim::IDLE_DOWN); break;
		}
		break;

	case State::WALKING:
		switch (look)
		{
		case Look::LEFT:   SetAnimation((int)PlayerAnim::WALK_LEFT); break;
		case Look::RIGHT:  SetAnimation((int)PlayerAnim::WALK_RIGHT); break;
		case Look::UP:     SetAnimation((int)PlayerAnim::WALK_UP); break;
		case Look::DOWN:   SetAnimation((int)PlayerAnim::WALK_DOWN); break;
		}
		break;

	case State::PUSH:
		switch (look)
		{
		case Look::LEFT:   SetAnimation((int)PlayerAnim::PUSH_LEFT); break;
		case Look::RIGHT:  SetAnimation((int)PlayerAnim::PUSH_RIGHT); break;
		case Look::UP:     SetAnimation((int)PlayerAnim::PUSH_UP); break;
		case Look::DOWN:   SetAnimation((int)PlayerAnim::PUSH_DOWN); break;
		}
		break;

	case State::DEAD:
		// Aquí podries posar una animació de mort si tens
		break;
	}
	
	// Important: crida a la funció base perquè l'animació es processi
	Entity::Update();
	if (map && !hasWon && map->CheckDiamondLines()) {
		hasWon = true;
		state = State::IDLE;  // Aturem el jugador
		Stop();               // Congela animació i moviment

		LOG("HAS GUANYAT");
	}
}

Point Player::GetFrontTilePos(int dx, int dy) const
{
	// Obtenim la hitbox actual del jugador
	AABB box = GetHitbox();

	// Calculem la posició (en píxels) davant segons la direcció
	float frontX = box.pos.x + dx * TILE_SIZE;
	float frontY = box.pos.y + dy * TILE_SIZE;

	// Convertim la posició en píxels a posició en tile (coordenades enters)
	int tileX = static_cast<int>(frontX) / TILE_SIZE;
	int tileY = static_cast<int>(frontY) / TILE_SIZE;

	return Point(tileX, tileY);
}

void Player::Move()
{
	AABB box = GetHitbox();

	// Variables auxiliares
	int dx = 0, dy = 0;
	bool isMoving = false;
	bool pushing = IsKeyDown(KEY_SPACE);
	bool breaking = IsKeyPressed(KEY_X); // NUEVO: Romper bloque al presionar X

	// Detectar direcció
	if (IsKeyDown(KEY_LEFT)) { dx = -1; look = Look::LEFT;  currentDirection = PlayerAnim::WALK_LEFT;  isMoving = true; }
	else if (IsKeyDown(KEY_RIGHT)) { dx = 1;  look = Look::RIGHT; currentDirection = PlayerAnim::WALK_RIGHT; isMoving = true; }
	else if (IsKeyDown(KEY_UP)) { dy = -1; look = Look::UP;    currentDirection = PlayerAnim::WALK_UP;    isMoving = true; }
	else if (IsKeyDown(KEY_DOWN)) { dy = 1;  look = Look::DOWN;  currentDirection = PlayerAnim::WALK_DOWN;  isMoving = true; }

	// Si no ens estem movent
	if (!isMoving) {
		currentDirection = PlayerAnim::NONE;
		state = State::IDLE;
	}

	// Obtener posición del tile frente al jugador
	Point frontTile = GetFrontTilePos(dx, dy);

	// NUEVO: romper bloque si se presiona X
	if (breaking) {
		if (map->BreakBlockAt(frontTile.x, frontTile.y)) {
			LOG("Bloque roto frente al jugador en (%d, %d)", frontTile.x, frontTile.y);
		}
		else {
			LOG("No se pudo romper el bloque en (%d, %d)", frontTile.x, frontTile.y);
		}
	}

	// Calcular hitbox simulado
	AABB newBox = box;
	newBox.pos.x += dx * PLAYER_SPEED;
	newBox.pos.y += dy * PLAYER_SPEED;

	bool noCollision = true;
	if (dx == -1) noCollision = !map->TestCollisionWallLeft(newBox);
	else if (dx == 1) noCollision = !map->TestCollisionWallRight(newBox);
	else if (dy == -1) noCollision = !map->TestCollisionWallUp(newBox);
	else if (dy == 1) noCollision = !map->TestCollisionWallDown(newBox);

	if (isMoving) {
		if (noCollision) {
			pos.x += dx * PLAYER_SPEED;
			pos.y += dy * PLAYER_SPEED;
			state = State::WALKING;
		}
		else if (pushing) {
			AABB blockBox(Point(frontTile.x * TILE_SIZE, frontTile.y * TILE_SIZE), TILE_SIZE, TILE_SIZE);
			if (map->TryPushBlock(blockBox, dx, dy, enemyManager)) {
				pos.x += dx * PLAYER_SPEED;
				pos.y += dy * PLAYER_SPEED;
				state = State::PUSH;
			}
			else {
				state = State::IDLE;
			}
		}
		else {
			state = State::IDLE;
		}
	}
}


void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}