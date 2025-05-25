
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

	// Comença el cooldown quan reps dany
	isDamageCooldownActive = true;
	damageCooldownTimer = 0.0f;

	// Aquí pots posar so o animacions d'haver rebut dany
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
	// Intentem començar a destruir un bloc si es prem la tecla corresponent
	TryDestroyBlock();

	// Si estem en procés de destruir un bloc, actualitzem l'animació de destrucció
	if (isDestroying)
	{
		const int totalFrames = 8;
		int frameDuration = destroyAnimDurationFrames / totalFrames;

		int frameIndex = destroyFrameCounter / frameDuration;
		if (frameIndex >= totalFrames) frameIndex = totalFrames - 1;

		// Posem el tile d'animació segons el frame actual
		Tile tileFrame = static_cast<Tile>(static_cast<int>(Tile::ICEBREAK_1) + frameIndex);
		map->SetTile(blockToDestroyTile.x, blockToDestroyTile.y, tileFrame);
		destroyFrameCounter++;

		// Quan acaba l'animació, fem desaparèixer el bloc i tornem a l'estat IDLE
		if (destroyFrameCounter >= destroyAnimDurationFrames)
		{
			map->SetTile(blockToDestroyTile.x, blockToDestroyTile.y, Tile::EMPTY);
			isDestroying = false;
			state = State::IDLE;
		}

		// No fer res més mentre es trenca el bloc
		return;
	}

	// Moviment i animacions normals quan no estem destruint
	Move();

	// Selecció d'animació segons estat i direcció
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
		// Opcional: animació de mort
		break;
	}

	// Actualitza l'animació de l'entity base
	Entity::Update();

	// Comprovació si s'ha complert condició de victòria
	if (map && !hasWon && map->CheckDiamondLines())
	{
		hasWon = true;
		state = State::IDLE;
		Stop(); // Atura moviment i animació
		LOG("HAS GUANYAT");
	}
}

Point Player::GetFrontTilePos(int dx, int dy) const
{
	AABB box = GetHitbox();

	float frontX = 0.0f;
	float frontY = 0.0f;

	if (dx > 0) // dreta
	{
		frontX = box.pos.x + PLAYER_PHYSICAL_WIDTH + dx * TILE_SIZE;
		frontY = box.pos.y + PLAYER_PHYSICAL_HEIGHT / 2;
	}
	else if (dx < 0) // esquerra
	{
		frontX = box.pos.x - TILE_SIZE;
		frontY = box.pos.y + PLAYER_PHYSICAL_HEIGHT / 2;
	}
	else if (dy > 0) // baix
	{
		frontX = box.pos.x + PLAYER_PHYSICAL_WIDTH / 2;
		frontY = box.pos.y + PLAYER_PHYSICAL_HEIGHT + dy * TILE_SIZE;
	}
	else if (dy < 0) // amunt
	{
		frontX = box.pos.x + PLAYER_PHYSICAL_WIDTH / 2;
		frontY = box.pos.y - TILE_SIZE;
	}
	else
	{
		// Si no hi ha direcció, centre del jugador
		frontX = box.pos.x + PLAYER_PHYSICAL_WIDTH / 2;
		frontY = box.pos.y + PLAYER_PHYSICAL_HEIGHT / 2;
	}

	int tileX = static_cast<int>(frontX) / TILE_SIZE;
	int tileY = static_cast<int>(frontY) / TILE_SIZE;

	return Point(tileX, tileY);
}

void Player::Move()
{
	AABB box = GetHitbox();

	// Variables auxiliars
	int dx = 0, dy = 0;
	bool isMoving = false;
	bool pushing = IsKeyDown(KEY_SPACE);

	// Detectar direcció
	if (IsKeyDown(KEY_LEFT)) { dx = -1; look = Look::LEFT;  currentDirection = PlayerAnim::WALK_LEFT;  isMoving = true; }
	else if (IsKeyDown(KEY_RIGHT)) { dx = 1;  look = Look::RIGHT; currentDirection = PlayerAnim::WALK_RIGHT; isMoving = true; }
	else if (IsKeyDown(KEY_UP)) { dy = -1; look = Look::UP;    currentDirection = PlayerAnim::WALK_UP;    isMoving = true; }
	else if (IsKeyDown(KEY_DOWN)) { dy = 1;  look = Look::DOWN;  currentDirection = PlayerAnim::WALK_DOWN;  isMoving = true; }

	// Si no ens estem movent
	if (!isMoving) {
		currentDirection = PlayerAnim::NONE;
		state = State::IDLE;
		return;
	}

	// Obtenim tile davant i el seu AABB
	Point frontTile = GetFrontTilePos(dx, dy);
	AABB blockBox(Point(frontTile.x * TILE_SIZE, frontTile.y * TILE_SIZE), TILE_SIZE, TILE_SIZE);

	// Calculem hitbox simulat
	AABB newBox = box;
	newBox.pos.x += dx * PLAYER_SPEED;
	newBox.pos.y += dy * PLAYER_SPEED;

	// Si no hi ha col·lisió
	bool noCollision = true;
	if (dx == -1) noCollision = !map->TestCollisionWallLeft(newBox);
	else if (dx == 1) noCollision = !map->TestCollisionWallRight(newBox);
	else if (dy == -1) noCollision = !map->TestCollisionWallUp(newBox);
	else if (dy == 1) noCollision = !map->TestCollisionWallDown(newBox);

	if (noCollision) {
		// Moure sense empènyer
		pos.x += dx * PLAYER_SPEED;
		pos.y += dy * PLAYER_SPEED;
		state = State::WALKING;
	}
	else if (pushing) {
		// Si volem empènyer i es pot moure el bloc
		if (map->TryPushBlock(blockBox, dx, dy,enemyManager)) {
			pos.x += dx * PLAYER_SPEED;
			pos.y += dy * PLAYER_SPEED;
			state = State::PUSH;
		}
		else {
			state = State::IDLE;
		}
	}
	else {
		// No podem moure ni empènyer
		state = State::IDLE;
	}
}
void Player::TryDestroyBlock()
{
	if (!map) return;

	// No fer res si ja estem destruint, estem morts o hem guanyat
	if (isDestroying || state == State::DEAD || hasWon)
		return;

	// Comprova si la tecla per trencar (E) està pressionada
	if (IsKeyPressed(KEY_E))
	{
		// Determina la direcció que mira el jugador
		int dx = 0, dy = 0;
		switch (look)
		{
		case Look::RIGHT: dx = 1; dy = 0; break;
		case Look::LEFT:  dx = -1; dy = 0; break;
		case Look::UP:    dx = 0; dy = -1; break;
		case Look::DOWN:  dx = 0; dy = 1; break;
		}

		// Obté la posició del tile davant el jugador
		Point frontTile = GetFrontTilePos(dx, dy);

		// Obté el tipus de tile que hi ha davant
		Tile tileType = map->GetTile(frontTile.x, frontTile.y);

		// Si és un bloc BLUEB, comença la destrucció
		if (tileType == Tile::BLUEB)
		{
			// Canvia el tile a ICEBREAK_1 per iniciar l'animació
			map->SetTile(frontTile.x, frontTile.y, Tile::ICEBREAK_1);

			isDestroying = true;
			blockToDestroyTile = frontTile;
			destroyFrameCounter = 0.5;  // Reinicia el comptador d'animació
			state = State::DESTROYING;
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