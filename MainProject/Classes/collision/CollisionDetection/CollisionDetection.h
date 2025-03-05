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
		//����p
		bool detection = false;

		constexpr float NINTY = 90.0f;

		//�������ݒ�
		Vector2 axis[4]
		{ 
			CF::ChangeAngleToVector(objectOne.GetAngle()),        //object1�̌����Ă����
			CF::ChangeAngleToVector(objectOne.GetAngle() + NINTY),//object1�̌����Ă�������琂������
			CF::ChangeAngleToVector(objectTwo.GetAngle()),        //object2�̌����Ă����
			CF::ChangeAngleToVector(objectTwo.GetAngle() + NINTY) //object2�̌����Ă�������琂������
		};

		Vector2 sizeHalf[2]
		{ 
			//x.�����Ă�����̕ӂ̒���||y.�����Ă�������琂���̕ӂ̒���
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