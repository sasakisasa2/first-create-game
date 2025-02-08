#include "PlantAnimation.h"

void PlantAnimation::Initialize(Vector2 setPlantPosition)
{
	animationWideScale = 0;
	animationHydeScale = 0;
	animationStopCount = 0;
	position[0] = setPlantPosition;
	sprite.size.x = GetSpriteOriginallySize().x;
	sprite.size.y = GetSpriteOriginallySize().y;
}

void PlantAnimation::Update(Vector2 playerPosition) 
{
	const float spriteSizeX = GetSpriteSize().x;
	const float spriteSizeY = GetSpriteSize().y;
	spriteRECT = RectWH(spriteSizeX * animationWideScale,
						spriteSizeY * animationHydeScale,
						spriteSizeX,
						spriteSizeY);
	
	if (playerPosition.x - position[0].x < 0.0f){ effect = SpriteEffects_None; }
	else { effect = SpriteEffects_FlipHorizontally; }

	if (animationStopCount != 2)
	{
		bool isAnimation = animationTimer.TimeMeasurement(GetAnimationSwitchTime());
		if (!isAnimation) { return; }
	}	
	animationWideScale += 1;
	animationTimer.TimerReSet();
	if (animationWideScale <= 2) { return; }	
	if (animationHydeScale == 1) 
	{
		animationStopCount += 1;
		animationWideScale = 2;
		return;
	}
	animationStopCount += 1;
	animationWideScale = 0;
	animationHydeScale = 1;
}

bool PlantAnimation::AnimationEndCheck()
{
	if (animationStopCount < 2) { return false; }
	return true;
}
