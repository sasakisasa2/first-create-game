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
		//”»’è—p
		bool detection = false;

		constexpr float NINTY = 90.0f;

		//•ª—£²İ’è
		Vector2 axis[4]
		{ 
			CF::ChangeAngleToVector(objectOne.GetAngle()),        //object1‚ÌŒü‚¢‚Ä‚é•ûŒü
			CF::ChangeAngleToVector(objectOne.GetAngle() + NINTY),//object1‚ÌŒü‚¢‚Ä‚é•ûŒü‚©‚ç‚’¼•ûŒü
			CF::ChangeAngleToVector(objectTwo.GetAngle()),        //object2‚ÌŒü‚¢‚Ä‚é•ûŒü
			CF::ChangeAngleToVector(objectTwo.GetAngle() + NINTY) //object2‚ÌŒü‚¢‚Ä‚é•ûŒü‚©‚ç‚’¼•ûŒü
		};

		Vector2 sizeHalf[2]
		{ 
			//x.Œü‚¢‚Ä‚é•ûŒü‚Ì•Ó‚Ì’·‚³||y.Œü‚¢‚Ä‚é•ûŒü‚©‚ç‚’¼‚Ì•Ó‚Ì’·‚³
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