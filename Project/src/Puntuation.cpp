#include "Puntuation.h"
#include "Globals.h"
#include "Entity.h"
#include "Sprite.h"
#include "LogMessages.h"

Puntuation::Puntuation(const Point& p) : Entity(p, PUNTUATION_SIZE, PUNTUATION_SIZE, PUNTUATION_SIZE, PUNTUATION_SIZE)
{
	pos = p;
	haveToRender = false;
}

Puntuation::~Puntuation()
{

}

AppStatus Puntuation::Initialise()
{
	const int n = PUNTUATION_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(ResourceType::IMG_PUNTUATIONS, "resources/sprites/PuntuationX2.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(ResourceType::IMG_PUNTUATIONS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for puntuation sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);

	sprite->SetNumberAnimations((int)Puntuations::NUM_ANIMATIONS);
	sprite->SetAnimationDelay((int)Puntuations::P_100, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_100, { 0 * n, 1 * n, n, n });
	sprite->AddKeyFrame((int)Puntuations::P_100, { 0 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_200, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_200, { 0 * n, 0 * n, n, n });
	sprite->AddKeyFrame((int)Puntuations::P_200, { 0 * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_300, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_300, { 1 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_400, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_400, { 1 * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_500, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_500, { 2 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_700, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_700, { 3 * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_800, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_800, { 2 * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_1000, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_1000, { 0 * n, 2 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_1600, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_1600, { 3 * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_2000, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_2000, { 1 * n, 2 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_3000, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_3000, { 2 * n, 2 * n, n, n });
	sprite->SetAnimationDelay((int)Puntuations::P_5000, ANIM_DELAY);
	sprite->AddKeyFrame((int)Puntuations::P_5000, { 3 * n, 2 * n, n, n });
	return AppStatus::OK;

	sprite->SetAnimation((int)Puntuations::P_100);
}

void Puntuation::Update()
{
	float currentTime = static_cast<float>(GetTime());
	float elapsedTime = currentTime - timeWhenShowed;
	
	if (elapsedTime > TIME_PUNT_SHOWED)
	{
		haveToRender = false;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Puntuation::ShowPuntuation(Point position, Puntuations punt)
{
	SetAnimation(static_cast<int>(punt));
	timeWhenShowed = static_cast<float>(GetTime());
	pos = { position.x - PUNTUATION_SIZE / 2, position.y - PUNTUATION_SIZE / 2 };
	haveToRender = true;
}

bool Puntuation::ReturnHaveToRender()
{
	return haveToRender;
}


void Puntuation::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}

void Puntuation::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(ResourceType::IMG_PUNTUATIONS);

	render->Release();
}