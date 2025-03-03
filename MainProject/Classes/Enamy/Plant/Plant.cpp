#include "Plant.h"

void Plant::Load(DirectXTK::Sprite& setSprite, DirectXTK::Sprite& setAttackSprite)
{
	animation.Load(setSprite);
	attack.Load(setAttackSprite);
}

void Plant::Initialize() 
{
	spawn.Spawn();
	animation.Initialize(spawn.GetPosition());
	attack.Initialize(spawn.GetPosition());
}

void Plant::Update(DirectX::SimpleMath::Vector2 playerPosition) 
{
	animation.Update(playerPosition);

	if (animation.AnimationEndCheck()==true)
	{
		attack.Update(playerPosition);
	}

	plantCollisionInfo.
		SetSquareCorner(
			spawn.GetPosition(),
			GetSpriteSize(),
			Vector2::Zero,
			0.0f,
			0.0f
		);

	for (int bulletNumber = 0; bulletNumber < ATTACK_MAX; bulletNumber++)
	{
		attackCollisionInfo.
			SetSquareCorner(
				attack.GetBulletPosition(bulletNumber),
				GetAttackSpriteSize(),
				attack.GetMoveVector(bulletNumber),
				attack.GetAngle(bulletNumber),
				GetAttackSpeed()
			);
	}
}

void Plant::Render(DirectX::SpriteBatch* spriteBatch) 
{
	animation.Render(spriteBatch, 0);
	if (animation.AnimationEndCheck() == true)
	{
		for (int attackCount = 0; attackCount < attack.ATTACK_MAX; attackCount++) 
		{
			attack.Render(spriteBatch,attackCount);
		}
	}
}
