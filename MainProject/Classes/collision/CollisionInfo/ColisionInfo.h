#pragma once
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Base\pch.h"

using namespace DirectX;

class CollisionInfo
{
private:
	SimpleMath::Vector2 squareCorner[4];
public:
	/// <summary>
	/// 当たり判定の為の位置保存用
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="vector">向かう方向</param>
	void SetSquareCorner(SimpleMath::Vector2 position, SimpleMath::Vector2 vector,float angle)
	{

	}
};