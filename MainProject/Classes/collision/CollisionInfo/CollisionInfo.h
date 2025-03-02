#pragma once
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Base\pch.h"

using namespace DirectX;
using namespace SimpleMath;

class CollisionInfo
{
private:
	Vector2 squareCorner[4];
public:
	/// <summary>
	/// 当たり判定の為の位置保存用
	/// </summary>
	/// <param name="position">位置         </param>
	/// <param name="size">    サイズ       </param>
	/// <param name="vector">  進む方向     </param>
	/// <param name="angle">   向いてる角度 </param>
	/// <param name="speed">   速度         </param>
	void SetSquareCorner(Vector2 position, Vector2 size,Vector2 vector, float angle, float speed)
	{
		//計算用定数
		const float   NINETY = 90;
		const float   MINUS  = -1;
		const Vector2 SIZE_HALF = size / 2;

		//1F後の位置
		const Vector2 ONE_FRAME_POSITION = position + (vector * speed);

		//サイズから逆算した角への角度
		const Vector2 CORNER_ANGLE = CF::RectangleCornerAngle(SIZE_HALF);
		//ここ→︻   への角度
		const float ANGLE_ONE   = CF::ChangeVectorToAngle(vector,angle) + (NINETY + CORNER_ANGLE.x);
		//ここ→︼   への角度
		const float ANGLE_TWO   = CF::ChangeVectorToAngle(vector, angle) - (NINETY + CORNER_ANGLE.x);
		//︻←ここ   への角度
		const float ANGLE_THREE = CF::ChangeVectorToAngle(vector, angle) + CORNER_ANGLE.y;
		//︼←ここ   への角度
		const float ANGLE_FOUR  = CF::ChangeVectorToAngle(vector, angle) - CORNER_ANGLE.y;

		//角への距離
		const float RADIUS = CF::Distance(Vector2(SIZE_HALF.x,0.0f), Vector2(0.0f,SIZE_HALF.y));

		squareCorner[0] = CF::SquareMovement(ANGLE_ONE  , position          , RADIUS);
		squareCorner[1] = CF::SquareMovement(ANGLE_TWO  , position          , RADIUS);
		squareCorner[2] = CF::SquareMovement(ANGLE_THREE, ONE_FRAME_POSITION, RADIUS);
		squareCorner[3] = CF::SquareMovement(ANGLE_FOUR , ONE_FRAME_POSITION, RADIUS);
	}
};