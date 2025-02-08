#include"AttackBit.h"
using namespace SimpleMath;

void AttackBit::Load(DirectXTK::Sprite& bitSprite, DirectXTK::Sprite& bitAttackSprite){
	move  .Load(bitSprite      );
	attack.Load(bitAttackSprite);
}

void  AttackBit::Initialize(float theta, bool directionRotation, bool attackDirection) {
	move  .Initialize(theta, directionRotation);
	attack.Initialize(theta, directionRotation, attackDirection);
}

void  AttackBit::Update(Vector2 centerPosition){
	move  .Update(centerPosition);
	attack.Update(centerPosition);
}

void AttackBit::Render(DirectX::SpriteBatch* spriteBatch){
	attack.Render(spriteBatch, attack.BIT_ATTACK_MAX);
	move  .Render(spriteBatch, 1                    );
}
