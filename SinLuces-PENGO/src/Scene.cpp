#include "Scene.h"
#include "Snobee.h"
#include "ResourceManager.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene(ResourceManager& data)
	: resMan(data),
	player(nullptr),
	level(nullptr),
	enemies(nullptr),
	shots(nullptr),
	debug(DebugMode::OFF) {
	
	// Inicialitzem camera al cos del constructor
	camera.target = { 0, 0 };
	camera.offset = { 0, MARGIN_GUI_Y };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (shots != nullptr)
	{
		delete shots;
		shots = nullptr;
	}
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create shot manager
	shots = new ShotManager();
	if (shots == nullptr)
	{
		LOG("Failed to allocate memory for Shot Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (shots->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Shot Manager");
		return AppStatus::ERROR;
	}

	//Create level
	level = new TileMap();
	if (level == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	//Load level (0 = random map, 1–3 = fijos)
	if (LoadLevel(0) != AppStatus::OK)
	{
		LOG("Failed to load level");
		return AppStatus::ERROR;
	}




	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	enemies->SetTileMap(level);
	player->SetEnemyManager(enemies);
	return AppStatus::OK;
}

AppStatus Scene::LoadLevel(int stage)
{

	if (level == nullptr) {
		LOG("Error: level is null in LoadLevel()");
		return AppStatus::ERROR;
	}

	int size;
	int x, y;
	int i = 0;
	Tile tile;
	Point pos;
	int* map = nullptr;
	Object* obj;
	AABB hitbox, area;

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;

	if (stage == 0) {
		if (level->GenerateRandomMap(LEVEL_WIDTH, LEVEL_HEIGHT, enemies) != AppStatus::OK) {
			LOG("Failed to generate random map");
			return AppStatus::ERROR;
		}
		player->InitScore();
		return AppStatus::OK;
	}

	if (stage == 1)
	{
		map = new int[size] {

		 	    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 200, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 200, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 200, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 18, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 18, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	
				1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			
			};	
	}
	else if (stage == 3)
	{
		map = new int[size] {
			    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 200, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0,18, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	Point debug_pos = { 5 * TILE_SIZE, 5 * TILE_SIZE };
	Point debug_area_pos = debug_pos + Point(-48, -32);
	AABB debug_area(debug_area_pos, 160, 96);


	//Entities and objects
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			pos.x = x * TILE_SIZE;
			pos.y = y * TILE_SIZE + TILE_SIZE - 1;

			if (tile == Tile::PLAYER)
			{
				player->SetPos(pos);
			}
			else if (tile == Tile::SNOBEE)
			{
				LOG("SNOBEE detectado en el mapa en (%d, %d)", x, y);
				Point area_pos = pos + Point(-48, -32);
				AABB area(area_pos, 160, 96);
				enemies->Add(pos, EnemyType::SNOBEE, area);
			}
			else if (tile == Tile::PURPLE_STAR)
			{
				LOG("🌟 Star encontrada en (%d, %d)", x, y);
				obj = new Object(pos, ObjectType::STAR);
				if (obj == nullptr) return AppStatus::ERROR;
				objects.push_back(obj);
			}
			++i;
		}
	}

	// Posición inicial por si acaso (opcional si ya se hace arriba)
	int middle_x = 8;
	int middle_y = 8;
	pos = { middle_x * TILE_SIZE, (middle_y + 2) * TILE_SIZE };
	player->SetPos(pos);

	// Limpiar marcadores del mapa
	level->ClearObjectEntityPositions();
	delete[] map;

	return AppStatus::OK;
	}


	void Scene::Update()
	{
		Point p1, p2;
		AABB playerHitbox = player->GetHitbox();

		// Switch between debug modes
		if (IsKeyPressed(KEY_F1))
		{
			debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
		}
		if (IsKeyPressed(KEY_F10)) {
			godMode = !godMode; // activa/desactiva
			if (godMode) {
				LOG("God Mode ACTIVAT");
			}
			else 
			{
				LOG("God Mode DESACTIVAT");

			}
		}
		if (IsKeyPressed(KEY_F8)) {
			LoadLevel(0); // mapa aleatorio
		}

		if (!godMode) {
			CheckPlayerEnemyCollisions();
		}
		
		player->Update();

		// Actualitza altres coses (nivell, enemics, etc.)
		level->Update();
		enemies->Update(player->GetHitbox());
		shots->Update(player->GetHitbox());

		// Debug levels instantly
		if (IsKeyPressed(KEY_ONE))      LoadLevel(1);
		else if (IsKeyPressed(KEY_TWO)) LoadLevel(2);
		else if (IsKeyPressed(KEY_THREE)) LoadLevel(3);

		CheckObjectCollisions();

		

		// Check collisions between player and enemies
		int currentHealth = player->GetHealth();
		if (enemies->CheckCollisionWithPlayer(playerHitbox, *player))
		{

			player->SetHealth(currentHealth);
			if (player->GetHealth() <= 0)
			{
				scene_state = SceneState::LOSE;
			}
		}
		player->UpdateDamageCooldown(GetFrameTime());
		shots->Update(playerHitbox);

		if (scene_state == SceneState::PLAYING)
		{
			// Check diamond line condition
			if (!diamondLineDetected)
			{
				if (level->CheckDiamondLines())
				{
					diamondLineDetected = true;
					diamondTimer = 0.0f;  // Reset timer
				}
			}
			else
			{
				diamondTimer += GetFrameTime();
				if (diamondTimer >= diamondCooldown)
				{
					scene_state = SceneState::WIN;
				}
			}
		}

		if (IsKeyPressed(KEY_F2))
		{
			scene_state = SceneState::WIN;
		}
		else if (IsKeyPressed(KEY_F3))
		{
			scene_state = SceneState::LOSE;
		}
		else if ((scene_state == SceneState::WIN || scene_state == SceneState::LOSE) && IsKeyPressed(KEY_ENTER))
		{
			LoadLevel(1);
			scene_state = SceneState::PLAYING;
			return;
		}

		if (scene_state != SceneState::PLAYING) return;
		
	}


void Scene::Render()
{

	if (scene_state == SceneState::WIN || scene_state == SceneState::LOSE)
	{
		const Texture2D* img = nullptr;
		ResourceManager& data = ResourceManager::Instance();

		if (scene_state == SceneState::WIN)  img = data.GetTexture(Resource::IMG_WIN);
		else                                 img = data.GetTexture(Resource::IMG_LOSE);

		if (img != nullptr)
		{
			DrawTexture(*img, 0, 0, WHITE);
		}
		return;
	}

	BeginMode2D(camera);

	level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		enemies->Draw();
		player->Draw();
		shots->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		enemies->DrawDebug();
		player->DrawDebug(GREEN);
		shots->DrawDebug(GRAY);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckObjectCollisions()
{
	AABB playerHitbox = player->GetHitbox(); 

	for (Enemy* enemy : enemies->GetAll())
	{
		if (playerHitbox.TestAABB(enemy->GetHitbox()))
		{
			if (player->CanTakeDamage()) {
				player->TakeDamage(1);
				player->StartDamageCooldown();
			}
			break;
		}
	}
}
void Scene::CheckPlayerEnemyCollisions()
{
	AABB playerHitbox = player->GetHitbox();

	for (Enemy* enemy : enemies->GetAll())  
	{
		AABB enemyHitbox = enemy->GetHitbox();

		if (playerHitbox.TestAABB(enemyHitbox))
		{
			if (!godMode)  // només fer mal si no estem en God Mode
				player->TakeDamage(1);

			player->TakeDamage(1); // El jugador perd 1 vida
			break; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	enemies->Release();
	shots->Clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::SetFont(Font* font)
{
	customFont = font;
}
void Scene::RenderGUI() const
{
	if (customFont != nullptr) {
		DrawTextEx(*customFont, "1P", { 80, 12 }, 16, 1, SKYBLUE);
		DrawTextEx(*customFont, TextFormat("%d", player->GetScore()), { 100, 12 }, 16, 1, LIGHTGRAY);
	}
	const Texture2D* heartTex = ResourceManager::Instance().GetTexture(Resource::IMG_LIVES);

	if (heartTex == nullptr)
	{
		// Si la textura no està carregada, podem sortir o dibuixar un missatge d'error
		DrawText("Error: Heart texture not loaded", 10, 20, 8, RED);
		return;
	}

	int currentHealth = player->GetHealth();
	const int heartSize = 12; // Depenent de la mida real del teu PNG
	const int spacing = 2;    // Espai entre cors
	const int xStart = 10;
	const int y = 12;

	for (int i = 0; i < currentHealth; ++i)
	{
		DrawTexture(*heartTex, xStart + i * (heartSize + spacing), y, WHITE);
	}
}