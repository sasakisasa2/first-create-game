#pragma once
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include"..\CollisionInfo\CollisionInfo.h"

class CollisionInfo;

namespace CD
{
	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="objectOne">判定のしたいオブジェクト元</param>
	/// <param name="objectTwo">判定のしたいオブジェクト</param>
	/// <returns>当たっている場合true 当たってない場合false</returns>
	bool CollisionDetection(CollisionInfo& objectOne, CollisionInfo& objectTwo)
	{
		if (CF::Distance(objectOne.GetPosition(), objectTwo.GetPosition()) < 100.0f) { return false; }

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

		//x.向いてる方向の辺の長さ||y.向いてる方向から垂直の辺の長さ
		Vector2 sizeHalf[2]
		{ 
			Vector2(
				CF::Distance(objectOne.GetSquareCorner(0),objectOne.GetSquareCorner(2)) / 2.0f,
				CF::Distance(objectOne.GetSquareCorner(1),objectOne.GetSquareCorner(3)) / 2.0f
			),
			Vector2(
				CF::Distance(objectTwo.GetSquareCorner(0),objectTwo.GetSquareCorner(2)) / 2.0f,
				CF::Distance(objectTwo.GetSquareCorner(1),objectTwo.GetSquareCorner(3)) / 2.0f
			)
		};

		//分離軸から垂直に位置する辺の長さの半分
		Vector2 radian[4]
		{
			#pragma region 分離軸1(object1の向いてる方向)
			
			Vector2
			(
				//分離軸とベクトルが同じなので、ベクトル方向の四角形のサイズ半分
				sizeHalf[0].x,
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				axis[0].Dot(axis[2] * sizeHalf[1].x) + axis[0].Dot(axis[3] * sizeHalf[1].y)
			),

			#pragma endregion
			#pragma region 分離軸2(object1の向いてる方向の垂直方向)

			Vector2
			(
				//分離軸とベクトルが同じなので、ベクトル方向の四角形のサイズ半分
				sizeHalf[0].y,
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				axis[1].Dot(axis[2] * sizeHalf[1].x) + axis[1].Dot(axis[3] * sizeHalf[1].y)
			),

			#pragma endregion
			#pragma region 分離軸3(object2の向いてる方向)

			Vector2
			(
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				axis[2].Dot(axis[0] * sizeHalf[0].x) + axis[2].Dot(axis[1] * sizeHalf[0].y),
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				sizeHalf[1].x
			),

			#pragma endregion
			#pragma region 分離軸4(object2の向いてる方向の垂直方向)

			Vector2
			(
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				axis[3].Dot(axis[0] * sizeHalf[0].x) + axis[3].Dot(axis[1] * sizeHalf[0].y),
				//四角形の中心から垂直のベクトル二つの内積の和の合計
				sizeHalf[1].y
			)

			#pragma endregion
		};

		//分離軸上のオブジェクト同士の距離
		float interval[4]
		{
			axis[0].Dot(objectOne.GetPosition() - objectTwo.GetPosition()),
			axis[1].Dot(objectOne.GetPosition() - objectTwo.GetPosition()),
			axis[2].Dot(objectOne.GetPosition() - objectTwo.GetPosition()),
			axis[3].Dot(objectOne.GetPosition() - objectTwo.GetPosition()),
		};

		float axisDetectionCount = 0;
		for (int axisCount = 0; axisCount < 4; axisCount++)
		{
			//オブジェクト間の距離がオブジェクト二つの半投影線分をプラスした値が小さい場合判定
			if (interval[axisCount] < radian[axisCount].x + radian[axisCount].y)
			{
				axisDetectionCount++;
			}
		}

		//分離軸四つ全て接触している判定になった場合接触している
		if(axisDetectionCount == 4)
			detection = true;

		return detection;
	}
};