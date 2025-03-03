#include "PlantSpawn.h"

void PlantSpawn::Spawn()
{
	spawnPosition = Vector2(CF::SetRandomf(GetRandomRange()),
							DXTK->Screen.Height-GetSpriteSize().y/2);
}
