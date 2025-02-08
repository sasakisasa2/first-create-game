#include"PlantManager.h"

PlantManager::PlantManager():
	position(ATTACK_MAX, Vector2(0, 0)),
	color   (ATTACK_MAX, Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	angle   (ATTACK_MAX, 0.0f),
	effect  (SpriteEffects_None)
{
	SetRandomRange		   (Vector2(200.0f, 1000.0f));
	SetSpriteScale		   (2.0f);
	SetSpriteOriginallySize(Vector2(177.0f, 140.0f )*scale);
	SetSpriteSize		   (Vector2(originallySize.x/3.0f, originallySize.y / 2.0f));
	SetAnimationSwitchTime (0.25f);
	SetHp				   (5);
	SetAttackSpriteSize	   (Vector2(30,30));
	SetAttackInterval	   (0.1f);
	SetAttackSpeed         (300.0f);
}

void PlantManager::Load(DirectXTK::Sprite& newSprite)
{
	sprite = newSprite;
}

void PlantManager::Render(DirectX::SpriteBatch* spriteBatch, int count)
{
	spriteBatch->Draw(
		sprite.handle, sprite.size,    //画像の情報
		position[count],               //画像を出す場所
		&spriteRECT,	  	           //画像のどこに表示するか
		color[count],				   //色の間引き方(カラーマスク)
		angle[count] * Mathf::Deg2Rad, //画像の角度
		Vector2((spriteRECT.right - spriteRECT.left) / 2,
			    (spriteRECT.bottom - spriteRECT.top) / 2),//画像の軸
		1.0f,						   //画像の表示倍率
		effect						   //画像のエフェクト
	);
}


void PlantManager::SetRandomRange(Vector2 setRange)
{
	randomRange = setRange;
}

Vector2 PlantManager::GetRandomRange()
{
	return randomRange;
}

void PlantManager::SetSpriteOriginallySize(Vector2 spriteSize)
{
	originallySize = spriteSize;
}

Vector2 PlantManager::GetSpriteOriginallySize()
{
	return originallySize;
}

void PlantManager::SetSpriteSize(Vector2 spriteSize)
{
	size = spriteSize;
}

Vector2 PlantManager::GetSpriteSize()
{
	return size;
}

void PlantManager::SetSpriteScale(float setScale)
{
	scale = setScale;
}

float PlantManager::GetSpriteScale()
{
	return scale;
}

void PlantManager::SetAnimationSwitchTime(float setSwithTime)
{
	animationSwitchTime = setSwithTime;
}

float PlantManager::GetAnimationSwitchTime()
{
	return animationSwitchTime;
}

void PlantManager::SetHp(int setHp){
	plantHp= setHp;
}

int PlantManager::GetHp(){
	return plantHp;
}

void PlantManager::SetAttackSpriteSize(Vector2 setAttackSpriteSize){
	attackSize = setAttackSpriteSize;
}

Vector2 PlantManager::GetAttackSpriteSize(){
	return attackSize;
}

void PlantManager::SetAttackInterval(float AttackInterval){
	attackInterval = AttackInterval;
}

float PlantManager::GetAttackInterval(){
	return attackInterval;
}

void PlantManager::SetAttackSpeed(float AttackSpeed){
	attackSpeed = AttackSpeed;
}

float PlantManager::GetAttackSpeed(){
	return attackSpeed;
}

