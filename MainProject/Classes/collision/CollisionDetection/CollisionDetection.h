#pragma once
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"

#include"..\CollisionInfo\CollisionInfo.h"
class CollisionInfo;

class CD
{
private:

public:
	bool CollisionDetection(CollisionInfo& objectOne, CollisionInfo& objectTwo)
	{
		//判定用
		bool detection = false;

		constexpr float NINTY = 90.0f;

		//分離軸設定
		Vector2 axis[4]
		{ 
			CF::ChangeAngleToVector(objectOne.GetAngle()),        //object1の向いてる方向
			CF::ChangeAngleToVector(objectOne.GetAngle() + NINTY),//object1の向いてる方向から垂直方向
			CF::ChangeAngleToVector(objectTwo.GetAngle()),        //object2の向いてる方向
			CF::ChangeAngleToVector(objectTwo.GetAngle() + NINTY) //object2の向いてる方向から垂直方向
		};

		Vector2 sizeHalf[2]
		{ 
			//x.向いてる方向の辺の長さ||y.向いてる方向から垂直の辺の長さ
			Vector2(
				CF::Distance(objectOne.GetSquareCorner(0),objectOne.GetSquareCorner(2)),
				CF::Distance(objectOne.GetSquareCorner(1),objectOne.GetSquareCorner(3))
			),
			Vector2(
				CF::Distance(objectTwo.GetSquareCorner(0),objectTwo.GetSquareCorner(2)),
				CF::Distance(objectTwo.GetSquareCorner(1),objectTwo.GetSquareCorner(3))
			)
		};
	}
};