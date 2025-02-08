#include"BitMove.h"

using namespace SimpleMath;

void BitMove::Initialize(float theta, bool directionRotation) {

	sprite.size.x = GetSpriteSize().x;
	sprite.size.y = GetSpriteSize().y;

	if (directionRotation) {
		//画像方向変換用
		SetRotationAngle(90.0f);
		//回転方向変換
		SetOmega(GetOmega() * -1);
	}

	bitTheta = theta;
}

void BitMove::Update(Vector2 centerPosition) {
	//角度をプラスし回転させる
	bitTheta      += GetOmega();
	//角度を参照し座標を変更
	position[0]    = SquareMovement(bitTheta, centerPosition, GetDistance());
	//画像の角度変更
	angle[0]       = bitTheta + GetRotationAngle();
}

