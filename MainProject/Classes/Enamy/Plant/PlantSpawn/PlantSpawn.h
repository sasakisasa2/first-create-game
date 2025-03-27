#pragma once

#include"..\PlantManager\PlantManager.h"

using namespace DirectX;
using namespace SimpleMath;
class PlantSpawn :public PlantManager
{
private:
	Vector2 spawnPosition;
public:
	void Spawn();
	Vector2 GetPosition()
	{
		return spawnPosition;
	}
};