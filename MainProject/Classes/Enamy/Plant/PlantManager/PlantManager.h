#pragma once
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include <random>
#include<vector>

using namespace DirectX;
using namespace SimpleMath;

class PlantManager 
{
private:
	Vector2 randomRange;//200.0f, 1000.0f

	Vector2 originallySize;//177,144
	Vector2 size;//originallySize.x/3,originallySize.y/2
	float scale;//2

	float animationSwitchTime;//0.25
	int   plantHp;//5

	Vector2 attackSize;//30,30
	float attackInterval;
	float attackSpeed;

protected:

#pragma region îhê∂ÉNÉâÉXÇÃâÊëúï`âÊóp
	RECT spriteRECT;
	DirectXTK::Sprite    sprite;
	std::vector<Vector2> position;
	std::vector<Color>   color;
	std::vector<float>   angle;
	DX12::SpriteEffects  effect;
#pragma endregion

#pragma region gettersetter
	void SetRandomRange(Vector2 setRange);
	Vector2 GetRandomRange();
	void SetSpriteOriginallySize(Vector2 spriteSize);
	Vector2 GetSpriteOriginallySize();
	void SetSpriteSize(Vector2 spriteSize);
	Vector2 GetSpriteSize();
	void SetSpriteScale(float serScale);
	float GetSpriteScale();
	void SetAnimationSwitchTime(float setSwithTime);
	float GetAnimationSwitchTime();
	void SetHp(int setHp);
	int GetHp();
	void SetAttackSpriteSize(Vector2 setAttackSpriteSize);
	Vector2 GetAttackSpriteSize();
	void SetAttackInterval(float AttackInterval);
	float GetAttackInterval();
	void SetAttackSpeed(float AttackSpeed);
	float GetAttackSpeed();
#pragma endregion


	PlantManager();

public:
	static constexpr int ATTACK_MAX = 50;

	void Load(DirectXTK::Sprite& newSprite);
	/// <summary>
	/// âÊëúï`âÊ
	/// </summary>
	/// <param name="spriteBarch">SpriteBatch   </param>
	/// <param name="count">      âΩâÒï`âÊÇ∑ÇÈÇ©</param>
	void Render(DirectX::SpriteBatch* spriteBatch,int count);
};