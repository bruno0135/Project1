#include "EnemyManager.h"
#include "Snobee.h"


void EnemyManager::SetTileMap(TileMap* m)
{
	this->map = m;
}


EnemyManager::EnemyManager()
{
	shots = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/Sno-Bees/Sno-Bee 1.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy = nullptr;
	switch (type)
	{
	case EnemyType::SNOBEE:
		enemy = new SNOBEE(pos, SNOBEE_PHYSICAL_WIDTH, SNOBEE_PHYSICAL_HEIGHT, SNOBEE_FRAME_SIZE, SNOBEE_FRAME_SIZE);
		break;

	default:
		LOG("Unknown enemy type");
		break;
	}

	if (enemy->Initialise(pos, type, area, map) != AppStatus::OK)
	{
		LOG("Failed to initialisse the enemy");
		delete enemy;
		return;
	}

	enemies.push_back(enemy);
}


AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SNOBEE)
	{
		width = SNOBEE_PHYSICAL_WIDTH;
		height = SNOBEE_PHYSICAL_HEIGHT;
	}

	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
bool EnemyManager::CheckCollisionWithPlayer(AABB& playerHitbox, int& playerHealth)
{
	for (Enemy* enemy : enemies)
	{
		// Suposem que Enemy té funció GetHitbox() que retorna AABB
		if (playerHitbox.TestAABB(enemy->GetHitbox()))
		{
			// Resta una vida al jugador si en té encara
			if (playerHealth > 0)
			{
				playerHealth--;
				LOG("Jugador ha perdut una vida. Vides restants: %d", playerHealth);
			}
			return true; // Col·lisió detectada
		}
	}
	return false; // No hi ha col·lisió
}


void EnemyManager::Update(const AABB& player_hitbox)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(player_hitbox);
	}
}
std::vector<Enemy*>& EnemyManager::GetAll() {
	return enemies;
}

void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}