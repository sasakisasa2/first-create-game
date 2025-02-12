#pragma once
#include "..\BossManager\BossManager.h"
using namespace DirectX;
using namespace SimpleMath;
class BossMove :public BossManager
{
private:
	bool isMoveEnd;
public:
	BossMove() :isMoveEnd(true) 
	{ }
	Vector2 GetPosition() { return position[0]; }
	bool    GetIsMoveEnd() { return isMoveEnd;  }
	void    SetIsMoveEnd(bool setMove) { isMoveEnd = setMove; }

	void Update(float vectorY);
};
