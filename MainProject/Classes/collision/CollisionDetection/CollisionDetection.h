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
		//�I�u�W�F�N�g�P�̌����𕪗����Ƃ��Đݒ�
		bool detection = false;
		Vector2 axis = CF::ChangeAngleToVector(objectOne.GetAngle());
	}

};