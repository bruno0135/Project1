#pragma once
#include "Entity.h"

enum class Puntuations {
	P_100, P_200, P_300, P_400, P_500, P_700, P_800, P_1000, P_1600, P_2000, P_3000, P_5000, NUM_ANIMATIONS
};


class Puntuation : public Entity
{
	
public:
	Puntuation(const Point& p);
	~Puntuation();
	
	void Update();
	void ShowPuntuation(Point pos, Puntuations puntuation);
	virtual AppStatus Initialise();
	void Release();
	bool ReturnHaveToRender();

protected:
	void SetAnimation(int id);
	bool haveToRender;
	float timeWhenShowed;
};