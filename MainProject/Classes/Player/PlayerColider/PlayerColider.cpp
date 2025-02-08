#include"PlayerColider.h"
using namespace SimpleMath;
void Colider::Initialize()
{
	sprite.size.x = GetSpriteSize() / 3;
	sprite.size.y = GetSpriteSize() / 3;
}

void Colider::Update(Vector2 playerPosition) { position[0] = playerPosition; }