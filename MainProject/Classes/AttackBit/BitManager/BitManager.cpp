#include"BitManager.h"

BitManager::BitManager():
	position(BIT_ATTACK_MAX, Vector2(0, 0)),
	color   (BIT_ATTACK_MAX, Vector4(1.0f,1.0f,1.0f,1.0f)),
	angle   (BIT_ATTACK_MAX, 0.0f)
{
	SetRotationAngle(-90.0f);				  //回転方向に応じて画像を回転させるための変数設定
	SetSpriteSize(Vector2(30.0f, 30.0f));	  //画像サイズ
	SetDistance(100.0f);					  //中心からの距離
	SetOmega(0.5f);							  //回転速度
	SetAttackSpriteSize(Vector2(30.0f,30.0f));//攻撃画像のサイズ
	SetAttackSpace(Vector2(5.0f,5.0f))  ;	  //x,攻撃全体の曲がり具合||y,弾同士の間隔
}

#pragma region アクセサ

#pragma region 共通アクセサ

#pragma region 回転角度設定用アクセサ
void  BitManager::SetRotationAngle(float setAngle) {
	rotationAngle = setAngle;
}
float BitManager::GetRotationAngle() {
	return rotationAngle;
}
#pragma endregion

#pragma endregion

#pragma region BitMoveのアクセサ

#pragma region Bitのサイズ
void    BitManager::SetSpriteSize(Vector2 setSize)
{
	spriteSize = setSize;
}
Vector2 BitManager::GetSpriteSize()
{
	return spriteSize;
}
#pragma endregion

#pragma region 中心座標からの距離
void  BitManager::SetDistance(float setDistance)
{
	distance = setDistance;
}
float BitManager::GetDistance()
{
	return distance;
}
#pragma endregion

#pragma region 回転速度
void  BitManager::SetOmega(float setOmega)
{
	omega = setOmega;
}
float BitManager::GetOmega()
{
	return omega;
}
#pragma endregion

#pragma endregion

#pragma region BitAttackのアクセサ

#pragma region 攻撃サイズ
void    BitManager::SetAttackSpriteSize(Vector2 setAttackSize)
{
	attackSpriteSize = setAttackSize;
}
Vector2 BitManager::GetAttackSpriteSize()
{
	return attackSpriteSize;
}
#pragma endregion

#pragma region 弾同士の間隔
void    BitManager::SetAttackSpace(Vector2 setAttackSpace)
{
	attackSpace = setAttackSpace;
}
Vector2 BitManager::GetAttackSpace()
{
	return attackSpace;
}
#pragma endregion

#pragma endregion

#pragma endregion


Vector2 BitManager::SquareMovement(float theta,Vector2 centerPosition,float distance)
{
	//指定された角度を関数用に値を設定
	double angle = theta/57.295791f;
	Vector2 differencePosition;
	//座標を参照し位置を移動
	differencePosition.x = centerPosition.x + distance * cos(angle);
	differencePosition.y = centerPosition.y + distance * sin(angle);
	return differencePosition;
}

void BitManager::Load(DirectXTK::Sprite& newSprite)
{
	sprite = newSprite;
}

void BitManager::Render(DirectX::SpriteBatch* spriteBatch, int count)
{
	for (int number = 0; number < count; number++) 
	{
		spriteBatch->Draw(
			sprite.handle, sprite.size, position[number], //描画
			nullptr,									  //画像のどこに表示するか
			color[number],                                //色の間引き方(カラーマスク)
			angle[number] * Mathf::Deg2Rad,               //回転の角度
			Vector2(sprite.size.x / 2, sprite.size.y / 2),//軸
			1.0f										  //画像サイズ(Spriteのサイズではない)
		);
	}
}
