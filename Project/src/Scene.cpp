#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "LogMessages.h"

Scene::Scene(Game* g, int sP, int sL, int l) : startPuntuation(sP), startLives(sL), lvl(l)
{
	game = g;
	player = nullptr;
	blinky = nullptr;
	pinky = nullptr;
	inky = nullptr;
	clyde = nullptr;
	level = nullptr;
	navMesh = nullptr;
	puntuation1 = nullptr;
	puntuation2 = nullptr;

	returnMainMenu = false;
	goNextLevel = false;
	died = false;
	won = false; 
	fruitSpawned = false;

	camera.target = { 0, 0 };				
	camera.offset = { MARGIN_GUI_X, MARGIN_GUI_Y };	
	camera.rotation = 0.0f;					
	camera.zoom = 1.0f;						

	totalPelets = 0;
	peletsCollected = 0;
	munch = 1;
	ghostEaten = 0;
	pacmanSpawnPos = { 0,0 };
	startWaitTime = 0;
	waitTime = 0;

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (blinky != nullptr)
	{
		blinky->Release();
		delete blinky;
		blinky = nullptr;
	}
	if (pinky != nullptr)
	{
		pinky->Release();
		delete pinky;
		pinky = nullptr;
	}
	if (inky != nullptr)
	{
		inky->Release();
		delete inky;
		inky = nullptr;
	}
	if (clyde != nullptr)
	{
		clyde->Release();
		delete clyde;
		clyde = nullptr;
	}
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}
	if (navMesh != nullptr)
	{
		delete navMesh;
		navMesh = nullptr;
	}
	if (puntuation1 != nullptr)
	{
		puntuation1->Release();
		delete puntuation1;
		puntuation1 = nullptr;
	}
	if (puntuation2 != nullptr)
	{
		puntuation2->Release();
		delete puntuation2;
		puntuation2 = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
AppStatus Scene::Init()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(ResourceType::IMG_ITEMS, "resources/sprites/ObjectsX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadTexture(ResourceType::IMG_LETTERS, "resources/sprites/LettersX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_MUNCH_1, "resources/sounds/FX/munch_1.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_MUNCH_2, "resources/sounds/FX/munch_2.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::MUSIC_START, "resources/sounds/Music/game_start.ogg") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_RETREATING, "resources/sounds/FX/retreating.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	
	if (data.LoadSound(ResourceType::SOUND_POWER_PELLET, "resources/sounds/FX/power_pellet.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_1, "resources/sounds/FX/siren_1.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_2, "resources/sounds/FX/siren_2.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_3, "resources/sounds/FX/siren_3.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_4, "resources/sounds/FX/siren_4.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_SIREN_5, "resources/sounds/FX/siren_5.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_EAT_GHOST, "resources/sounds/FX/eat_ghost.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_DEAD, "resources/sounds/FX/death.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	if (data.LoadSound(ResourceType::SOUND_EAT_FRUIT, "resources/sounds/FX/eat_fruit.wav") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	startMusic = data.GetSound(ResourceType::MUSIC_START);
	power_pellet = data.GetSound(ResourceType::SOUND_POWER_PELLET);
	retreating = data.GetSound(ResourceType::SOUND_RETREATING);
	siren_1 = data.GetSound(ResourceType::SOUND_SIREN_1);
	siren_2 = data.GetSound(ResourceType::SOUND_SIREN_2);
	siren_3 = data.GetSound(ResourceType::SOUND_SIREN_3);
	siren_4 = data.GetSound(ResourceType::SOUND_SIREN_4);
	siren_5 = data.GetSound(ResourceType::SOUND_SIREN_5);
	munch_1 = data.GetSound(ResourceType:: SOUND_MUNCH_1);
	munch_2 = data.GetSound(ResourceType:: SOUND_MUNCH_2);
	eat_ghost = data.GetSound(ResourceType::SOUND_EAT_GHOST);
	dead = data.GetSound(ResourceType::SOUND_DEAD);
	eat_fruit = data.GetSound(ResourceType::SOUND_EAT_FRUIT);
	lettersTexture = data.GetTexture(ResourceType::IMG_LETTERS);

	//Create player
	player = new Player({ 0, 0 }, PlayerState::IDLE, Directions::RIGHT);
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

	//Create blinky
	blinky = new Blinky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
	if (blinky == nullptr)
	{
		LOG("Failed to allocate memory for blinky");
		return AppStatus::ERROR;
	}
	//Initialise blinky
	if (blinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise blinky");
		return AppStatus::ERROR;
	}
	//Create pinky
	pinky = new Pinky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
	if (pinky == nullptr)
	{
		LOG("Failed to allocate memory for pinky");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (pinky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise pinky");
		return AppStatus::ERROR;
	}
	inky = new Inky({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
	if (inky == nullptr)
	{
		LOG("Failed to allocate memory for inky");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (inky->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise inky");
		return AppStatus::ERROR;
	}
	clyde = new Clyde({ 0, 0 }, GhostState::SCATTLE, Directions::RIGHT);
	if (clyde == nullptr)
	{
		LOG("Failed to allocate memory for clyde");
		return AppStatus::ERROR;
	}
	//Initialise pinky
	if (clyde->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise clyde");
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
	//Create navMesh
	navMesh = new NavMesh();
	if (navMesh == nullptr)
	{
		LOG("Failed to allocate memory for Navigation Mesh");
		return AppStatus::ERROR;
	}

	//Create puntuation
	puntuation1 = new Puntuation({ 0,0 });
	if (navMesh == nullptr)
	{
		LOG("Failed to allocate memory for Navigation Mesh");
		return AppStatus::ERROR;
	}
	if (puntuation1->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise puntuation");
		return AppStatus::ERROR;
	}
	puntuation2 = new Puntuation({ 0,0 });
	if (navMesh == nullptr)
	{
		LOG("Failed to allocate memory for Navigation Mesh");
		return AppStatus::ERROR;
	}
	if (puntuation2->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise puntuation");
		return AppStatus::ERROR;
	}

	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}


	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	player->InitScore(startPuntuation);
	player->InitLives(startLives);
	blinky->SetTileMap(level);
	blinky->SetNavMesh(navMesh);
	blinky->SetPlayer(player);
	pinky->SetTileMap(level);
	pinky->SetNavMesh(navMesh);
	pinky->SetPlayer(player);
	inky->SetTileMap(level);
	inky->SetNavMesh(navMesh);
	inky->SetPlayer(player);
	inky->SetBlinky(blinky);
	clyde->SetNavMesh(navMesh);
	clyde->SetTileMap(level);
	clyde->SetPlayer(player);

	started = false;
	ghostState = GhostState::SCATTLE;
	lastStateChangeTime = 0;
	if (lvl == 0) PlaySound(*startMusic);
	else waitTime = TIME_TO_START;
	return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	int* navMeshMap = nullptr;
	Object* obj;

	ClearLevel();

	if (stage == 1)
	{
		size = LEVEL_WIDTH * LEVEL_HEIGHT;
		map = new int[size] {
			0,	0,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	2,	2,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	52,	0,	0,	5,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	51,	0,	0,	5,	0,	0,	0,	0,	4,	0,	52,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	5,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	0,	0,	4,	0,	0,	0,	0,	5,	0,	0,	0,	0,	4,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	5,	0,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	0,	4,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24, 33,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	0,	3,	3,	3,	3,	3,	8,	0,	0,	0,	0,	9,	8,	0,	0,	0,	0,	9,	3,	3,	3,	3,	3,	0,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	101,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	23,	24,	24,	24,	35,	50,	50,	50,	50,	36,	24,	24,	24,	25,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			1,	1,	1,	1,	1,	1,	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	51,	0,	0,	9,	8,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	9,	8,	0,	51,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	1,	1,	1,	1,	1,	1,
			1,	53,	-1,-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,-1,	-1,	-1, 54,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	-1,	-1,	-1,	-1,	-1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	-1,	-1,	-1,	-1, -1,	1,
			1,	1,	1,	1,	1,	1,	24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 31,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	30,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	1,	1,	1,	1,	1,	1,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	29,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	28,	27,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	102,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	51,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	51,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	30, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	51,	0,	0,	9,	8,	0,	0,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	0,	0,	0,	9,	8,	0,	51,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	31,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	0,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	0,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	100,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	52,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	0,	0,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	52,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	5,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	7,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	4,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	5,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	0,	0,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	4,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	5,	4,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	5,	4,	0,	51,	0,	0,	0,	0,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	2,	2,	2,	2,	0,	0,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	5,	4,	0,	0,	0,	0,	7,	2,	2,	2,	2,	2,	0,	0,	2,	2,	2,	2,	2,	2,	2,	2,	2,	6,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	9,	8,	0,	51,	0,	0,	9,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	8,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	51,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	32, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 33,	0,	0,	0,	0,	0,	0
		};
	
		size = (LEVEL_WIDTH / 2) * (LEVEL_HEIGHT / 2);
		navMeshMap = new int[size] {
			1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
		};
		
		player->InitScore(startPuntuation);
		player->InitLives(startLives);
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				pacmanSpawnPos = pos;
				player->SetPos(pacmanSpawnPos);
			}
			else if (tile == Tile::GHOST)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				ghostSpawnPos = pos;
				blinky->SetPos(ghostSpawnPos);
				blinky->StartWalking(Directions::RIGHT);
				pinky->SetPos(ghostSpawnPos);
				pinky->StartWalking(Directions::LEFT);
				inky->SetPos(ghostSpawnPos);
				inky->StartWalking(Directions::RIGHT);
				clyde->SetPos(ghostSpawnPos);
				clyde->StartWalking(Directions::LEFT);
			}
			else if (tile == Tile::SMALL_PELET)
			{
				pos.x = x * TILE_SIZE + SMALL_OBJECT_SIZE / 2;
				pos.y = y * TILE_SIZE + SMALL_OBJECT_SIZE / 2;
				Point pHitBox;
				pHitBox.x = SMALL_OBJECT_SIZE / 4;
				pHitBox.y = SMALL_OBJECT_SIZE / 4;
				obj = new Object(pos, pHitBox, ObjectType::SMALL_PELET, SMALL_OBJECT_SIZE, SMALL_OBJECT_SIZE / 2 );
				objects.push_back(obj);
				map[i] = 0;
				totalPelets++;
			}
			else if (tile == Tile::LARGE_PELET)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;
				obj = new Object(pos, ObjectType::LARGE_PELET, MEDIUM_OBJECT_SIZE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::TP_LEFT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;

				obj = new Object(pos, ObjectType::LEFT_TELEPORTER, TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE * 4);
				objects.push_back(obj);
				map[i] = -1;
			}
			else if (tile == Tile::TP_RIGHT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;

				obj = new Object(pos, ObjectType::RIGHT_TELEPORTER, TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE * 4);
				objects.push_back(obj);
				map[i] = -1;
			}
			else if (tile == Tile::FRUIT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE;

				fruitSpawnPoint = pos;

				map[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	
	//Nav mesh map
	navMesh->Load(navMeshMap, LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2);

	delete map;
	delete navMeshMap;

	return AppStatus::OK;
}
void Scene::Update()
{
	if (!IsSoundPlaying(*startMusic) && !started)
	{
		started = true;
		startWaitTime = static_cast<float>(GetTime());
	}

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}

	float currentTime = static_cast<float>(GetTime());
	float elapsedTime = currentTime - startWaitTime;

	if (elapsedTime > waitTime)
	{
		waitTime = 0;
		if (started && !died && !won)
		{
			level->Update();
			player->Update();
			UpdateGhostState();
			blinky->Update();
			pinky->Update();
			inky->Update();
			clyde->Update();
			puntuation1->Update();
			puntuation2->Update();
			CheckCollisions();
			PlaySounds();
		}
		else if (won)
		{
			Won();
		}
		else if (died)
		{
			if (IsSoundPlaying(*dead)) player->Update();
			else
			{
				if (player->GetLives() > 0) ReloadScene();
				else returnMainMenu = true;
			}
		}

		CheckEndLevel();
	}
}
void Scene::PlayerDie()
{
	died = true;
	player->Die();
	player->DecrLives(1);
	PlaySound(*dead);
}
void Scene::Win()
{
	won = true;
	startWinTimer = static_cast<float>(GetTime());
	player->SetAnimation((int)PlayerAnim::BITE_DOWN);
}
void Scene::Won()
{
	float currentTime = static_cast<float>(GetTime());
	float elapsedTime = currentTime - startWinTimer;

	if (elapsedTime < TIME_WIN)
	{
		level->ChangeMap(static_cast<int>(elapsedTime * 2) % 2 == 0); //TRUE TO WHITE FALSE TO BLUE
	}
	else GoNextLevel();

}
void Scene::ReloadScene()
{
	started = false;
	died = false;
	player->SetPos(pacmanSpawnPos);	
	player->Reload();
	waitTime = TIME_TO_START;

	ghostState = GhostState::SCATTLE;
	float percent = static_cast<float>(peletsCollected) / static_cast<float>(totalPelets) * 100;
	if (percent >= BLINKY_PERCENT_TO_PERMANENTLY_CHASE) blinky->ChangeCommonState(GhostState::CHASE);

	blinky->SetPos(ghostSpawnPos);
	if (percent < BLINKY_PERCENT_TO_PERMANENTLY_CHASE) blinky->ChangeCommonState(ghostState);
	blinky->Reload();

	pinky->SetPos(ghostSpawnPos);
	pinky->ChangeCommonState(ghostState);
	pinky->Reload();

	inky->SetPos(ghostSpawnPos);
	inky->ChangeCommonState(ghostState);
	inky->Reload();

	clyde->SetPos(ghostSpawnPos);
	clyde->ChangeCommonState(ghostState);
	clyde->Reload();

	lastStateChangeTime = 0;
}
void Scene::PlaySounds()
{
	const Sound* currentSound = nullptr;

	if (blinky->GetState() == GhostState::EATEN || pinky->GetState() == GhostState::EATEN || inky->GetState() == GhostState::EATEN || clyde->GetState() == GhostState::EATEN)
	{
		currentSound = retreating;
	}
	else if (blinky->GetState() == GhostState::FRIGHTENED || pinky->GetState() == GhostState::FRIGHTENED || inky->GetState() == GhostState::FRIGHTENED || clyde->GetState() == GhostState::FRIGHTENED)
	{
		currentSound = power_pellet;
	}
	else
	{
		float percent = static_cast<float>(peletsCollected) / static_cast<float>(totalPelets) * 100;

		if (percent < 30)
		{
			currentSound = siren_1;
		}
		else if (percent < 60)
		{
			currentSound = siren_2;
		}
		else if (percent < 75)
		{
			currentSound = siren_3;
		}
		else if (percent < 90)
		{
			currentSound = siren_4;
		}
		else if (percent <= 100)
		{
			currentSound = siren_5;
		}
	}

	// Stop all sounds except the current one
	StopSoundsInException(currentSound);

	// Play the current sound if it's not already playing
	if (currentSound && !IsSoundPlaying(*currentSound))
	{
		PlaySound(*currentSound);
	}
}

void Scene::StopSoundsInException(const Sound* sound)
{
	if (retreating != sound && IsSoundPlaying(*retreating)) StopSound(*retreating);
	if (power_pellet != sound && IsSoundPlaying(*power_pellet)) StopSound(*power_pellet);
	if (siren_1 != sound && IsSoundPlaying(*siren_1)) StopSound(*siren_1);
	if (siren_2 != sound && IsSoundPlaying(*siren_2)) StopSound(*siren_2);
	if (siren_3 != sound && IsSoundPlaying(*siren_3)) StopSound(*siren_3);
	if (siren_4 != sound && IsSoundPlaying(*siren_4)) StopSound(*siren_4);
	if (siren_5 != sound && IsSoundPlaying(*siren_5)) StopSound(*siren_5);
}
void Scene::StopAllSounds()
{
	StopSound(*startMusic);
	StopSound(*retreating);
	StopSound(*power_pellet);
	StopSound(*siren_1);
	StopSound(*siren_2);
	StopSound(*siren_3);
	StopSound(*siren_4);
	StopSound(*siren_5);
	StopSound(*munch_1);
	StopSound(*munch_2);
	StopSound(*eat_ghost);
	StopSound(*eat_fruit);
	StopSound(*dead);
}
void Scene::Render()
{
	BeginMode2D(camera);

	level->Render();

	if (debug == DebugMode::SPRITES_AND_NAVMESH)
	{
		navMesh->DrawDebug(RED);
	}
	if (debug == DebugMode::SPRITES_AND_NAVMESH_ROUTES || debug == DebugMode::SPRITES_AND_NAVMESH)
	{
		blinky->DrawDebugNavmesh(RED);
		pinky->DrawDebugNavmesh(PINK);
		inky->DrawDebugNavmesh(SKYBLUE);
		clyde->DrawDebugNavmesh(ORANGE);
	}
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::SPRITES_AND_NAVMESH || debug == DebugMode::SPRITES_AND_NAVMESH_ROUTES || debug == DebugMode::SPRITES_AND_NAVMESH)
	{
		RenderObjects();
		player->Draw();
		
		if (!died && !won)
		{
			blinky->Draw();
			pinky->Draw();
			inky->Draw();
			clyde->Draw();
			if (puntuation1->ReturnHaveToRender())
				puntuation1->Draw();
			if (puntuation2->ReturnHaveToRender())
				puntuation2->Draw();
		}
		if (died && player->GetLives() == 0)
		{
			int n = LETTERS_SIZE;
			Rectangle  sourceRect = { 16 * n, 7 * n,(float)9 * n,(float)1 * n };
			DrawTextureRec(*lettersTexture, sourceRect, { LEVEL_WIDTH * 8 / 2 - sourceRect.width/2, LEVEL_HEIGHT * 8 / 2 + sourceRect.height * 1.5f} , WHITE);
		}
		if (IsSoundPlaying(*startMusic) || waitTime!= 0)
		{
			int n = LETTERS_SIZE;
			Rectangle  sourceRect = { 0 * n, 8 * n, (float)6 * n,(float)1 * n };
			DrawTextureRec(*lettersTexture, sourceRect, { LEVEL_WIDTH * 8 / 2 - sourceRect.width / 2, LEVEL_HEIGHT * 8 / 2 + sourceRect.height * 1.5f }, WHITE);
		}
	}
	level->RenderEmptys();
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		blinky->DrawDebug(RED);
		pinky->DrawDebug(RED);
		inky->DrawDebug(RED);
		clyde->DrawDebug(RED);
		DrawPixel(0, 0, WHITE);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	StopAllSounds();
	startMusic = nullptr; 
	retreating = nullptr;
	power_pellet = nullptr;
	siren_1 = nullptr;
	siren_2 = nullptr;
	siren_3 = nullptr;
	siren_4 = nullptr;
	siren_5 = nullptr;
	munch_1 = nullptr;
	munch_2 = nullptr;
	eat_ghost = nullptr;
	dead = nullptr;
	eat_fruit = nullptr;
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseSound(ResourceType::SOUND_SIREN_1);
	data.ReleaseSound(ResourceType::SOUND_SIREN_2);
	data.ReleaseSound(ResourceType::SOUND_SIREN_3);
	data.ReleaseSound(ResourceType::SOUND_SIREN_4);
	data.ReleaseSound(ResourceType::SOUND_SIREN_5);
	data.ReleaseSound(ResourceType::SOUND_POWER_PELLET);
	data.ReleaseSound(ResourceType::SOUND_RETREATING);
	data.ReleaseSound(ResourceType::SOUND_MUNCH_2);
	data.ReleaseSound(ResourceType::SOUND_MUNCH_1);
	data.ReleaseSound(ResourceType::MUSIC_START);
	data.ReleaseSound(ResourceType::SOUND_EAT_GHOST);
	data.ReleaseSound(ResourceType::SOUND_DEAD);
	data.ReleaseTexture(ResourceType::IMG_LETTERS);
	data.ReleaseSound(ResourceType::SOUND_EAT_FRUIT);

	ClearLevel();
}
void Scene::UpdateGhostState()
{
	if (lastStateChangeTime == 0) lastStateChangeTime = static_cast<float>(GetTime());
	float currentTime = static_cast<float>(GetTime());
	float elapsedTime = currentTime - lastStateChangeTime;

	float percent = static_cast<float>(peletsCollected) / static_cast<float>(totalPelets) * 100;
	float dificultyPercent = static_cast<float>(lvl) / 10;
	if (dificultyPercent > 1) dificultyPercent = 1;
	
	if (percent >= BLINKY_PERCENT_TO_PERMANENTLY_CHASE) blinky->ChangeCommonState(GhostState::CHASE);
	

	if (ghostState == GhostState::SCATTLE && elapsedTime >= TIME_IN_SCATTER - TIME_IN_SCATTER * dificultyPercent) {
		ghostState = GhostState::CHASE;
		if (percent < BLINKY_PERCENT_TO_PERMANENTLY_CHASE) blinky->ChangeCommonState(ghostState);
		pinky->ChangeCommonState(ghostState);
		inky->ChangeCommonState(ghostState);
		clyde->ChangeCommonState(ghostState);
		lastStateChangeTime = currentTime;
	}
	else if (ghostState == GhostState::CHASE && elapsedTime >= TIME_IN_CHASE) {
		ghostState = GhostState::SCATTLE;
		if (percent < BLINKY_PERCENT_TO_PERMANENTLY_CHASE) blinky->ChangeCommonState(ghostState);
		pinky->ChangeCommonState(ghostState);
		inky->ChangeCommonState(ghostState);
		clyde->ChangeCommonState(ghostState);
		lastStateChangeTime = currentTime;
	}
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, blinky_box, pinky_box, inky_box, clyde_box;

	player_box = player->GetHitbox();
	blinky_box = blinky->GetHitbox();
	pinky_box = pinky->GetHitbox();
	inky_box = inky->GetHitbox();
	clyde_box = clyde->GetHitbox();
	
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		ObjectType type = (*it)->GetObjectType();
		if (player_box.TestAABB(obj_box))
		{
			if (type == ObjectType::SMALL_PELET)
			{
				if (munch_1 != nullptr) {
					if (!IsSoundPlaying(*munch_1) && !IsSoundPlaying(*munch_2))
						if (munch == 1) 
						{ 
							PlaySound(*munch_1); 
							munch = 2; 
						}
						else if (munch == 2) 
						{ 
							PlaySound(*munch_2); 
							munch = 1; 
						}
				}
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
				peletsCollected++;
				if (peletsCollected == totalPelets)	Win();
				float percent = static_cast<float>(peletsCollected) / static_cast<float>(totalPelets) * 100;
				if (percent >= PERCENT_TO_FRUIT && !fruitSpawned)
				{
					int levelIndex = lvl % 8;
					Point pos;
					Object* obj;
					pos.x = fruitSpawnPoint.x;
					pos.y = fruitSpawnPoint.y;
					obj = new Object(pos, static_cast<ObjectType>(static_cast<int>(ObjectType::FIRST_FRUIT) + levelIndex), LARGE_OBJECT_SIZE);
					objects.push_back(obj);
					fruitSpawned = true;
				}
			}
			else if (type == ObjectType::LARGE_PELET)
			{
				if (blinky->GetState() != GhostState::EATEN) blinky->ChangeState(GhostState::FRIGHTENED);
				if (pinky->GetState() != GhostState::EATEN) pinky->ChangeState(GhostState::FRIGHTENED);
				if (inky->GetState() != GhostState::EATEN) inky->ChangeState(GhostState::FRIGHTENED);
				if (clyde->GetState() != GhostState::EATEN) clyde->ChangeState(GhostState::FRIGHTENED);
				delete* it;
				it = objects.erase(it);
				ghostEaten = 0;
			}
			else if (type == ObjectType::RIGHT_TELEPORTER)
			{
				player->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				player->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
			else if (type == ObjectType::FRUIT0)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT1)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT2)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT3)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT4)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT5)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT6)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
			else if (type == ObjectType::FRUIT7)
			{
				if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_fruit);
				EatFruitPuntuation(type, (*it)->GetCenterPosition());
				player->IncrScore((*it)->Points());
				delete* it;
				it = objects.erase(it);
			}
		}
		if (blinky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				blinky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				blinky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (pinky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				pinky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				pinky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (inky_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				inky->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				inky->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		if (clyde_box.TestAABB(obj_box))
		{
			if (type == ObjectType::RIGHT_TELEPORTER)
			{
				clyde->SetPos(Point({ LEFT_TP_POS_X, LEFT_TP_POS_Y }));
			}
			else if (type == ObjectType::LEFT_TELEPORTER)
			{
				clyde->SetPos(Point({ RIGHT_TP_POS_X,RIGHT_TP_POS_Y }));
			}
		}
		
		if (type == ObjectType::SMALL_PELET)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::LARGE_PELET)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::RIGHT_TELEPORTER)
		{
			it++;
		}
		else if (type == ObjectType::LEFT_TELEPORTER)
		{
			it++;
		}
		else if (type == ObjectType::FRUIT0)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT1)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT2)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT3)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT4)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT5)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT6)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}
		else if (type == ObjectType::FRUIT7)
		{
			if (!player_box.TestAABB(obj_box)) it++;
		}


	}

	if (player_box.TestAABB(blinky_box) && (float)player->CalculateDistance(blinky->GetPosition()) <DISTANCE_COLLISION)
	{
		
		if (blinky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			blinky->ChangeState(GhostState::EATEN); 
			if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_ghost);
			EatGhostPuntuation(blinky->GetCenterPosition());
		}
		else if (blinky->GetState() == GhostState::SCATTLE || blinky->GetState() == GhostState::CHASE) PlayerDie();
	}
	if (player_box.TestAABB(pinky_box) && (float)player->CalculateDistance(pinky->GetPosition()) < DISTANCE_COLLISION)
	{
		if (pinky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			pinky->ChangeState(GhostState::EATEN); 
			if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_ghost);
			EatGhostPuntuation(pinky->GetCenterPosition());
		}
		else if (pinky->GetState() == GhostState::SCATTLE || pinky->GetState() == GhostState::CHASE) PlayerDie();
	}
	if (player_box.TestAABB(inky_box) && (float)player->CalculateDistance(inky->GetPosition()) < DISTANCE_COLLISION)
	{
		if (inky->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			inky->ChangeState(GhostState::EATEN); 
			if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_ghost);
			EatGhostPuntuation(inky->GetCenterPosition());
		}
		else if (inky->GetState() == GhostState::SCATTLE || inky->GetState() == GhostState::CHASE) PlayerDie();
	}
	if (player_box.TestAABB(clyde_box) && (float)player->CalculateDistance(clyde->GetPosition()) < DISTANCE_COLLISION)
	{
		if (clyde->GetState() == GhostState::FRIGHTENED) //COMIDO
		{
			clyde->ChangeState(GhostState::EATEN); 
			if (!IsSoundPlaying(*eat_ghost)) PlaySound(*eat_ghost);
			EatGhostPuntuation(clyde->GetCenterPosition());
		}
		else if (clyde->GetState() == GhostState::SCATTLE || clyde->GetState() == GhostState::CHASE) PlayerDie();
	}

}
void Scene::CheckEndLevel()
{
	if (returnMainMenu) game->ReturnToMainMenu();
	else if (goNextLevel) game->GoNextLevel(player->GetScore(), player->GetLives(), lvl);
}
void Scene::ShowPuntuation(Point position, Puntuations puntuation)
{
	if (!puntuation1->ReturnHaveToRender()) puntuation1->ShowPuntuation(position, puntuation);
	else if (!puntuation2->ReturnHaveToRender()) puntuation2->ShowPuntuation(position, puntuation);
}
void Scene::EatGhostPuntuation(Point position)
{
	switch (ghostEaten)
	{
	case 0:
		ShowPuntuation(position, Puntuations::P_200);
		player->IncrScore(200);
		break;
	case 1:
		ShowPuntuation(position, Puntuations::P_400);
		player->IncrScore(400);
		break;
	case 2:
		ShowPuntuation(position, Puntuations::P_800);
		player->IncrScore(800);
		break;
	case 3:
		ShowPuntuation(position, Puntuations::P_1600);
		player->IncrScore(1600);
		break;
	default:
		break;
	}
	ghostEaten++;
}
void Scene::EatFruitPuntuation(ObjectType type, Point position)
{
	switch (type)
	{
	case ObjectType::FRUIT0:
		ShowPuntuation(position, Puntuations::P_100);
		player->IncrScore(200);
		break;
	case ObjectType::FRUIT1:
		ShowPuntuation(position, Puntuations::P_300);
		player->IncrScore(400);
		break;
	case ObjectType::FRUIT2:
		ShowPuntuation(position, Puntuations::P_500);
		player->IncrScore(800);
		break;
	case ObjectType::FRUIT3:
		ShowPuntuation(position, Puntuations::P_700);
		player->IncrScore(1600);
		break;
	case ObjectType::FRUIT4:
		ShowPuntuation(position, Puntuations::P_1000);
		player->IncrScore(200);
		break;
	case ObjectType::FRUIT5:
		ShowPuntuation(position, Puntuations::P_2000);
		player->IncrScore(400);
		break;
	case ObjectType::FRUIT6:
		ShowPuntuation(position, Puntuations::P_3000);
		player->IncrScore(800);
		break;
	case ObjectType::FRUIT7:
		ShowPuntuation(position, Puntuations::P_5000);
		player->IncrScore(1600);
		break;
	default:
		break;
	}
	ghostEaten++;
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	
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
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 20, LIGHTGRAY);
	const char* text = TextFormat("LIVES : %d", player->GetLives());
	int fontSize = 20;
	DrawText(TextFormat(text), WINDOW_WIDTH - MeasureText(text, fontSize) - 10, 10, fontSize, LIGHTGRAY);
}
void Scene::GoNextLevel()
{
	goNextLevel = true;
}