#pragma once

#include"..\PlantManager\PlantManager.h"
#include"..\PlantSpawn\PlantSpawn.h"
using namespace DirectX;
using namespace SimpleMath;
class PlantAttack:public PlantManager
{
private:
	CF::Timer intervalTimer;
	bool isShot[ATTACK_MAX];
	Vector2 distance[ATTACK_MAX];
	Vector2 plantPosition;
	Vector2 playerPosition;
	void Preparation();
	
public:

	void Initialize(Vector2 setPlantPosition);
	void Update(Vector2 playerPosition);

	float   GetAngle         (int bulletNumber) { return angle[bulletNumber];    }
	Vector2 GetMoveVector    (int bulletNumber) { return distance[bulletNumber]; }
	Vector2 GetBulletPosition(int bulletNumber) { return position[bulletNumber]; }
	bool    GetIsShot        (int bulletNumber) { return isShot[bulletNumber];   }

};