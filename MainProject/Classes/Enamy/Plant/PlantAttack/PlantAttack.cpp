#include "PlantAttack.h"

void PlantAttack::Initialize(Vector2 setPlantPosition)
{
	sprite.size.x = GetAttackSpriteSize().x;
	sprite.size.y = GetAttackSpriteSize().y;
	plantPosition = setPlantPosition;

	for (int attackCount=0; attackCount < ATTACK_MAX; attackCount++)
	{
		isShot  [attackCount] = false;
		position[attackCount] = plantPosition;
	}
}

void PlantAttack::Update(Vector2 newPlayerPosition)
{
	playerPosition = newPlayerPosition;

	Preparation();

		for (int attackCount = 0; attackCount < ATTACK_MAX; attackCount++)
		{
			if(isShot[attackCount])
			position[attackCount] += distance[attackCount] * GetAttackSpeed() * DXTK->Time.deltaTime;
		}

	spriteRECT = RectWH(0.0f,0.0f,sprite.size.x,sprite.size.y);
}

void PlantAttack::Preparation()
{
	bool isInterval = intervalTimer.TimeMeasurement(GetAttackInterval());
	if (!isInterval) 
	{
		return;
	}

	for (int attackCount = 0; attackCount < ATTACK_MAX; attackCount++) 
	{
		if (!isShot[attackCount]) 
		{
			isShot [attackCount] = true;
			position[attackCount] = plantPosition;
			distance[attackCount] = CF::DistanceCount(playerPosition,plantPosition);
			intervalTimer.TimerReSet();
			break;
		}
	}
}