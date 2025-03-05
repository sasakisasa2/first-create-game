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

		//x.�����Ă�����̕ӂ̒���||y.�����Ă�������琂���̕ӂ̒���
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

		//���������琂���Ɉʒu����ӂ̒����̔���
		Vector2 radian[4]
		{
			#pragma region ������1(object1�̌����Ă����)
			
			Vector2
			(
				//�������ƃx�N�g���������Ȃ̂ŁA�x�N�g�������̎l�p�`�̃T�C�Y����
				sizeHalf[0].x,
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				axis[0].Dot(axis[2] * sizeHalf[1].x) + axis[0].Dot(axis[3] * sizeHalf[1].y)
			),

			#pragma endregion
			#pragma region ������2(object1�̌����Ă�����̐�������)

			Vector2
			(
				//�������ƃx�N�g���������Ȃ̂ŁA�x�N�g�������̎l�p�`�̃T�C�Y����
				sizeHalf[0].y,
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				axis[1].Dot(axis[2] * sizeHalf[1].x) + axis[1].Dot(axis[3] * sizeHalf[1].y)
			),

			#pragma endregion
			#pragma region ������3(object2�̌����Ă����)

			Vector2
			(
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				axis[2].Dot(axis[0] * sizeHalf[0].x) + axis[2].Dot(axis[1] * sizeHalf[0].y),
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				sizeHalf[1].x
			),

			#pragma endregion
			#pragma region ������4(object2�̌����Ă�����̐�������)

			Vector2
			(
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				axis[3].Dot(axis[0] * sizeHalf[0].x) + axis[3].Dot(axis[1] * sizeHalf[0].y),
				//�l�p�`�̒��S���琂���̃x�N�g����̓��ς̘a�̍��v
				sizeHalf[1].y
			)

			#pragma endregion
		};

	}
};