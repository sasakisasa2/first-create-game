#include "PlantSpawn.h"

void PlantSpawn::Spawn()
{
	spawnPosition = Vector2(random.SetRandomf(GetRandomRange()),
							DXTK->Screen.Height-GetSpriteSize().y/2);
}
