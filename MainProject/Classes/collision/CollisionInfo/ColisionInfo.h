#pragma once
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Base\pch.h"

using namespace DirectX;
using namespace SimpleMath;

class CollisionInfo
{
private:
	:Vector2 squareCorner[4];
public:
	/// <summary>
	/// 当たり判定の為の位置保存用
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="vector">向かう方向</param>
	void SetSquareCorner(Vector2 position, Vector2 vector, Vector2 size, float angle, float speed)
	{
		//計算用定数
		const float   NINETY = 90;
		const float   MINUS  = -1;
		const Vector2 SIZE_HALF = size / 2;

		//サイズから逆算した角への角度
		const Vector2 CORNER_ANGLE = CF::RectangleCornerAngle(SIZE_HALF);
		//角への距離
		const float RADIUS = CF::Distance(Vector2(SIZE_HALF.x,0.0f), Vector2(0.0f,SIZE_HALF.y));
	}
};