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
		//îªíËóp
		bool detection = false;

		constexpr float NINTY = 90.0f;

		//ï™ó£é≤ê›íË
		Vector2 axis[4]
		{ 
			CF::ChangeAngleToVector(objectOne.GetAngle()),
			CF::ChangeAngleToVector(objectOne.GetAngle() + NINTY),
			CF::ChangeAngleToVector(objectTwo.GetAngle()),
			CF::ChangeAngleToVector(objectTwo.GetAngle() + NINTY),
		};
		Vector2 sizeHalf[2]{ objectOne.GetSize() / 2.0f,objectTwo.GetSize() / 2.0f };
	}
};