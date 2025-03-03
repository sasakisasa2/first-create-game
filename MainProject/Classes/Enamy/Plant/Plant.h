#pragma once
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Classes\Enamy\Plant\PlantManager\PlantManager.h"
#include"..\MainProject\Classes\Enamy\Plant\PlantAnimation\PlantAnimation.h"
#include"..\MainProject\Classes\Enamy\Plant\PlantAttack\PlantAttack.h"
#include"..\MainProject\Classes\Enamy\Plant\PlantSpawn\PlantSpawn.h"

class Plant:public PlantManager
{
private:
	PlantSpawn     spawn;
	PlantAnimation animation;
	PlantAttack    attack;

	CollisionInfo  plantCollisionInfo;
	CollisionInfo  attackCollisionInfo;
public:

	CollisionInfo GetCollisionInfo()       { return plantCollisionInfo;  }
	CollisionInfo GetAttackCollisionInfo() { return attackCollisionInfo; }

	void Load(DirectXTK::Sprite& setSprite,DirectXTK::Sprite& setAttackSprite);
	void Initialize();
	void Update(DirectX::SimpleMath::Vector2 playerPosition);
	void Render(DirectX::SpriteBatch* spriteBatch);
};