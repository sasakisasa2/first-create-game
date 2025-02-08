
#pragma once

#include"..\PlantManager\PlantManager.h"

using namespace DirectX;
using namespace SimpleMath;
class PlantAnimation :public PlantManager
{
private:
	CF::Timer animationTimer;
	Vector2   animationScale;
	int       animationWideScale;
	int       animationHydeScale;
	int       animationStopCount;
public:
	void Initialize(Vector2 setPlantPosition);
	void Update(Vector2 playerPosition);
	bool AnimationEndCheck();
};