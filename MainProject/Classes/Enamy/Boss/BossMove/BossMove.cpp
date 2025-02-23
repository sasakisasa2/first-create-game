#include"BossMove.h"

void BossMove::Update(float vectorY)
{
	if (isMoveEnd) { return; }
	position[0].y += GetMoveSpeed() * vectorY * DXTK->Time.deltaTime;
	if (position[0].y >= DXTK->Screen.Height - (GetBossSpriteSize().y / 2.0f)&& vectorY == 1.0f)
	{
		isMoveEnd = true;
		position[0].y = DXTK->Screen.Height - GetBossSpriteSize().y / 2.0f;
	}
	if (position[0].y <= 0.0f + (GetBossSpriteSize().y / 2.0f)&& vectorY == -1.0f)
	{
		isMoveEnd = true;
		position[0].y = 0.0f + GetBossSpriteSize().y / 2.0f;
	}
}
