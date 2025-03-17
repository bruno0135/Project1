#include "Ghost.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "LogMessages.h"

Ghost::Ghost(const Point& p, GhostState s, Directions d) :
	Entity(p, GHOST_PHYSICAL_WIDTH, GHOST_PHYSICAL_HEIGHT, GHOST_FRAME_SIZE, GHOST_FRAME_SIZE)
{
	state = s;
	direction = d;
	map = nullptr;
	score = 0; 
	speed = GHOST_SPEED;
	state = GhostState::SCATTLE;
	commonState = GhostState::SCATTLE;
}
Ghost::~Ghost()
{

}
AppStatus Ghost::Initialise()
{
	const int n = GHOST_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_GHOSTS, "resources/sprites/GhostsX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_GHOSTS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for ghost sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)GhostAnim::NUM_ANIMATIONS);
	sprite->SetAnimationDelay((int)GhostAnim::WALK_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { (float)0 * n, static_cast<float>(type) * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_RIGHT, { (float)1 * n, static_cast<float>(type) * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { (float)2 * n, static_cast<float>(type) * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_LEFT, { (float)3 * n, static_cast<float>(type) * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { (float)4 * n, static_cast<float>(type) * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_UP, { (float)5 * n, static_cast<float>(type) * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::WALK_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { (float)6 * n, static_cast<float>(type) * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::WALK_DOWN, { (float)7 * n, static_cast<float>(type) * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::FRIGHTENED, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::FRIGHTENED, { 0 * n, 4 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::FRIGHTENED, { 1 * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::FRIGHTENED_WHITE, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::FRIGHTENED_WHITE, { 2 * n, 4 * n, n, n });
	sprite->AddKeyFrame((int)GhostAnim::FRIGHTENED_WHITE, { 3 * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::EATEN_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EATEN_UP, { 6 * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::EATEN_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EATEN_DOWN, { 7 * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::EATEN_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EATEN_LEFT, { 5 * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)GhostAnim::EATEN_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)GhostAnim::EATEN_RIGHT, { 4 * n, 4 * n, n, n });

	stateChangeTime = 0;
	state = GhostState::SCATTLE;

	return AppStatus::OK;
}
void Ghost::Update()
{
	
}
void Ghost::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Ghost::SetNavMesh(NavMesh* nM)
{
	navMesh = nM;
}
bool Ghost::IsLookingRight() const
{
	return direction == Directions::RIGHT;
}
bool Ghost::IsLookingLeft() const
{
	return direction == Directions::LEFT;
}
bool Ghost::IsLookingUp() const
{
	return direction == Directions::UP;
}
bool Ghost::IsLookingDown() const
{
	return direction == Directions::DOWN;
}
void Ghost::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
GhostAnim Ghost::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (GhostAnim)sprite->GetAnimation();
}
void Ghost::StartWalkingLeft()
{
	direction = Directions::LEFT;
	if(state == GhostState::SCATTLE || state == GhostState::CHASE) SetAnimation((int)GhostAnim::WALK_LEFT);
	else if(state == GhostState::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED_WHITE) SetAnimation((int)GhostAnim::FRIGHTENED);
	else if(state == GhostState::EATEN) SetAnimation((int)GhostAnim::EATEN_LEFT);
}
void Ghost::StartWalkingRight()
{
	direction = Directions::RIGHT;
	if (state == GhostState::SCATTLE || state == GhostState::CHASE) SetAnimation((int)GhostAnim::WALK_RIGHT);
	else if (state == GhostState::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED_WHITE) SetAnimation((int)GhostAnim::FRIGHTENED);
	else if (state == GhostState::EATEN) SetAnimation((int)GhostAnim::EATEN_RIGHT);
}
void Ghost::StartWalkingUp()
{
	direction = Directions::UP;
	if (state == GhostState::SCATTLE || state == GhostState::CHASE) SetAnimation((int)GhostAnim::WALK_UP);
	else if (state == GhostState::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED_WHITE) SetAnimation((int)GhostAnim::FRIGHTENED);
	else if (state == GhostState::EATEN) SetAnimation((int)GhostAnim::EATEN_UP);
}
void Ghost::StartWalkingDown()
{
	direction = Directions::DOWN;
	if (state == GhostState::SCATTLE || state == GhostState::CHASE) SetAnimation((int)GhostAnim::WALK_DOWN);
	else if (state == GhostState::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED && GetAnimation() != GhostAnim::FRIGHTENED_WHITE) SetAnimation((int)GhostAnim::FRIGHTENED);
	else if (state == GhostState::EATEN) SetAnimation((int)GhostAnim::EATEN_DOWN);
}
void Ghost::UpdateStates()
{
	switch (state)
	{
	case GhostState::CHASE:
		if (commonState == GhostState::SCATTLE) 
			ChangeState(GhostState::SCATTLE);
		break;
	case GhostState::SCATTLE:
		if (commonState == GhostState::CHASE) 
			ChangeState(GhostState::CHASE);
		break;
	case GhostState::FRIGHTENED: {
		float currentTime = static_cast<float>(GetTime());
		float elapsedTime = currentTime - stateChangeTime;
		if (elapsedTime >= TIME_IN_FRIGHTENED) ChangeState(commonState);
		else {
			if (elapsedTime >= TIME_TO_BLINK)
			{
				float partOfSecond = fmod(elapsedTime, 1.0f);
				if (partOfSecond < 0.5f) {
					if (GetAnimation() != GhostAnim::FRIGHTENED_WHITE) SetAnimation(static_cast<int>(GhostAnim::FRIGHTENED_WHITE));
				}
				else {
					if (GetAnimation() != GhostAnim::FRIGHTENED) SetAnimation(static_cast<int>(GhostAnim::FRIGHTENED));
				}
			}
			else if (GetAnimation() == GhostAnim::FRIGHTENED_WHITE) SetAnimation(static_cast<int>(GhostAnim::FRIGHTENED));
		}
		break;
	}
	case GhostState::EATEN:
		if (!route.empty())
		{
			Point endPos = navMesh->GetPixelToGo(route.at(route.size()-1).movedIndex, GHOST_FRAME_SIZE);
			if (pos.x == endPos.x && pos.y == endPos.y)
			{
				if (commonState == GhostState::SCATTLE)
					ChangeState(GhostState::SCATTLE);
				else ChangeState(GhostState::CHASE);
			}
		}
		break;
	default:
		break;
	}
}
void Ghost::ChangeState(GhostState s)
{
	switch (s)
	{
	case GhostState::CHASE:
		state = GhostState::CHASE;
		speed = GHOST_SPEED;
		StartWalking(direction);
		break;
	case GhostState::SCATTLE:
		state = GhostState::SCATTLE;
		speed = GHOST_SPEED;
		StartWalking(direction);
		break;
	case GhostState::FRIGHTENED:
		state = GhostState::FRIGHTENED;
		speed = GHOST_SPEED/2;
		StartWalking(OppositeDirection(direction));
		stateChangeTime = static_cast<float>(GetTime());
		break;
	case GhostState::EATEN:
		state = GhostState::EATEN;
		speed = GHOST_SPEED/2;

		StartWalking(direction);
		route.clear();
		break;
	default:
		break;
	}
}

void Ghost::Move()
{
	bool collide = true;
	Point prev = pos;
	for (int i = 0; i < speed && collide; i++)
	{
		if (direction == Directions::RIGHT)
		{
			pos.x += (speed - i);
			if (!map->TestCollisionWallRight(GetHitbox())) collide = false;
		}
		else if (direction == Directions::LEFT)
		{
			pos.x += -(speed - i);
			if (!map->TestCollisionWallLeft(GetHitbox())) collide = false;
		}
		else if (direction == Directions::UP)
		{
			pos.y += -(speed - i);
			if (!map->TestCollisionWallUp(GetHitbox())) collide = false;
		}
		else if (direction == Directions::DOWN)
		{
			pos.y += (speed - i);
			if (!map->TestCollisionWallDown(GetHitbox())) collide = false;
		}

		if (collide) pos = prev;
	}

}
void Ghost::Reload()
{
	route.clear();
	ChangeState(GhostState::SCATTLE);
}
void Ghost::GoPath(Point point)
{
	while (true)
	{
		bool canFollowPath = false;
		std::vector<Directions> availableDirections = GetDirectionsCanMove();

		if (navMesh->CheckItsJustAbove(pos, GHOST_FRAME_SIZE) && (route.size() == 0 || ChangeRoute(point)))
		{
			route = navMesh->GetBestPath(GetCenterPosition().x, GetCenterPosition().y, point.x, point.y, direction); //Calcula la mejor ruta
		}

		if (!route.empty())
		{
			canFollowPath = true;
			Point nextPos = navMesh->GetPixelToGo(route.front().movedIndex, GHOST_FRAME_SIZE);

			if (pos.x == nextPos.x && pos.y == nextPos.y)
			{
				route.erase(route.begin());
			}

			if (!route.empty())
			{
				Point currentPos = navMesh->GetPixelToGo(route.front().startIndex, GHOST_FRAME_SIZE);
				if (pos.x == currentPos.x && pos.y == currentPos.y)
				{
					StartWalking(route.front().movementDir);
				}
			}
			else
			{
				canFollowPath = false;
				continue; 
			}
		}

		if (!canFollowPath)
		{
			route.clear();
			for (const auto& nextDir : availableDirections)
			{
				if (nextDir != direction && nextDir != OppositeDirection(direction)) // Avoid going backwards
				{
					StartWalking(nextDir);
					break;
				}
			}
		}
		break;
	}
}
GhostState Ghost::GetState()
{
	return state;
}
void Ghost::MoveRandomly()
{
	std::vector<Directions> availableDirections = GetDirectionsCanMove();
	route.clear();
	for (const auto& nextDir : availableDirections)
	{
		if (nextDir != direction && nextDir != OppositeDirection(direction)) // Avoid going backwards
		{
			StartWalking(nextDir);
			break; // Change direction only once
		}
	}
}

Directions Ghost::OppositeDirection(Directions dir)
{
	switch (dir)
	{
	case Directions::UP:
		return Directions::DOWN;
	case Directions::DOWN:
		return Directions::UP;
	case Directions::LEFT:
		return Directions::RIGHT;
	case Directions::RIGHT:
		return Directions::LEFT;
	default:
		return Directions::NONE; // En caso de que dir no sea una dirección válida
	}
}

std::vector<Directions> Ghost::GetDirectionsCanMove()
{
	std::vector<Directions> availableDirections;
	Point prev = pos;

	// Agregar todas las direcciones disponibles
	pos.y += -speed;
	if (!map->TestCollisionWallUp(GetHitbox())) availableDirections.push_back(Directions::UP);
	pos = prev;

	pos.y += speed;
	if (!map->TestCollisionWallDown(GetHitbox())) availableDirections.push_back(Directions::DOWN);
	pos = prev;

	pos.x += speed;
	if (!map->TestCollisionWallRight(GetHitbox())) availableDirections.push_back(Directions::RIGHT);
	pos = prev;

	pos.x += -speed;
	if (!map->TestCollisionWallLeft(GetHitbox())) availableDirections.push_back(Directions::LEFT);
	pos = prev;

	// Mezclar las direcciones disponibles de forma aleatoria
	std::random_device rd;
	std::default_random_engine rng(rd());
	std::shuffle(availableDirections.begin(), availableDirections.end(), rng);

	return availableDirections;
}

void Ghost::StartWalking(Directions d)
{
	if (d == Directions::UP) StartWalkingUp();
	else if (d == Directions::DOWN) StartWalkingDown();
	else if (d == Directions::RIGHT) StartWalkingRight();
	else if (d == Directions::LEFT) StartWalkingLeft();
}

void Ghost::ChangeCommonState(GhostState state)
{
	commonState = state;
}

void Ghost::DrawDebug(const Color& col)
{
	Entity::DrawHitbox(GREEN);
	DrawPixel(GetCenterPosition().x, GetCenterPosition().y, RED);
}

void Ghost::DrawDebugNavmesh(const Color& col)
{
	Point startPosition = navMesh->GetPathToPixels(navMesh->GetPathIndex(GetCenterPosition().x, GetCenterPosition().y).x, navMesh->GetPathIndex(GetCenterPosition().x, GetCenterPosition().y).y);

	if (route.size() != 0)
	{
		// Dibujar el camino desde el jugador al primer punto de la ruta
		Point firstPoint = navMesh->GetPathToPixels(route[0].movedIndex.x, route[0].movedIndex.y);
		DrawPathBetweenPoints(startPosition, firstPoint, col);


		// Dibujar el camino entre los puntos de la ruta
		for (size_t i = 0; i < route.size() - 1; i++)
		{
			Point start = navMesh->GetPathToPixels(route[i].movedIndex.x, route[i].movedIndex.y);
			Point end = navMesh->GetPathToPixels(route[i + 1].movedIndex.x, route[i + 1].movedIndex.y);
			DrawPathBetweenPoints(start, end, col);
		}

		// Dibujar el último punto de la ruta
		Point finalTarget = navMesh->GetPathToPixels(route.back().movedIndex.x, route.back().movedIndex.y);
		DrawRectangle(finalTarget.x, finalTarget.y, PATH_SIZE, PATH_SIZE, col);
	}
}

void Ghost::DrawPathBetweenPoints(Point start, Point end, const Color& col)
{
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int steps = std::max(abs(dx), abs(dy)); // Número de pasos basado en el mayor desplazamiento

	float xIncrement = static_cast<float>(dx) / static_cast<float>(steps);
	float yIncrement = static_cast<float>(dy) / static_cast<float>(steps);

	float x = static_cast<float>(start.x);
	float y = static_cast<float>(start.y);
	for (int i = 0; i <= steps; i++)
	{
		DrawRectangle(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), PATH_SIZE, PATH_SIZE, col);
		x += xIncrement;
		y += yIncrement;
	}
}


void Ghost::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_GHOSTS);

	render->Release();
}

bool Ghost::ChangeRoute(Point point)
{
	return !navMesh->CheckIfPointInIndex(point, route.at(route.size() - 1).movedIndex) && navMesh->ItsWalkable(point);
}

void Ghost::SetPlayer(Player* p)
{
	player = p;
}