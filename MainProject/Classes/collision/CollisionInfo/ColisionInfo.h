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
	/// �����蔻��ׂ̈̈ʒu�ۑ��p
	/// </summary>
	/// <param name="position">�ʒu</param>
	/// <param name="vector">����������</param>
	void SetSquareCorner(Vector2 position, Vector2 vector, Vector2 size, float angle, float speed)
	{
		//�v�Z�p�萔
		const float   NINETY = 90;
		const float   MINUS  = -1;
		const Vector2 SIZE_HALF = size / 2;

		//�T�C�Y����t�Z�����p�ւ̊p�x
		const Vector2 CORNER_ANGLE = CF::RectangleCornerAngle(SIZE_HALF);
		//�p�ւ̋���
		const float RADIUS = CF::Distance(Vector2(SIZE_HALF.x,0.0f), Vector2(0.0f,SIZE_HALF.y));
	}
};