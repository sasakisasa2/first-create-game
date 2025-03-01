#include"CF.h"

CF::Timer::Timer() : time(0) 
{}

void CF::Timer::TimerReSet()
{
	time = 0;
}

bool CF::Timer::TimeMeasurement(float timeLimit)
{
	time += DXTK->Time.deltaTime;
	if (time >= timeLimit) 
	{
		return true;
	}
	return false;
}


float CF::SetRandomf(SimpleMath::Vector2 randomRange)
{
	std::random_device rd;
	std::mt19937 random = std::mt19937(rd());
	std::uniform_real_distribution<float> other;
	other = std::uniform_real_distribution<float>(randomRange.x, randomRange.y);
	return other(random);
}

int CF::SetRandom(SimpleMath::Vector2 randomRange)
{
	std::random_device rd;
	std::mt19937 random = std::mt19937(rd());
	std::uniform_real_distribution<float> other;
	other = std::uniform_real_distribution<float>(randomRange.x, randomRange.y);
	return (int)other(random);
}

SimpleMath::Vector2 CF::DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo)
{
	//距離の計測	
	float distance = sqrt((positionOne.x - positionTwo.x) *
					      (positionOne.x - positionTwo.x) +
						  (positionOne.y - positionTwo.y) *
						  (positionOne.y - positionTwo.y));
	SimpleMath::Vector2 vector = (positionOne - positionTwo) / distance;
	return vector;
}

float CF::Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo)
{
	//距離の計測	
	float distance = sqrt((positionOne.x - positionTwo.x) *
						  (positionOne.x - positionTwo.x) +
						  (positionOne.y - positionTwo.y) *
						  (positionOne.y - positionTwo.y));
	return distance;
}

bool CF::PositionRangeOver(SimpleMath::Vector2 position, SimpleMath::Vector2 width, SimpleMath::Vector2 Height)
{
	bool isOver = false;
	if (position.x < width.x)
	{
		isOver = true;
	}
	if (position.x > width.y)
	{
		isOver = true;
	}
	if (position.y < Height.x)
	{
		isOver = true;
	}
	if (position.y > Height.y)
	{
		isOver = true;
	}
	return isOver;
}

float CF::ChangeVectorToAngle(SimpleMath::Vector2 vector,float angle)
{
	if (vector.x == 0.0f && vector.y == 0.0f) { return angle; }

	//ベクトルのXとYの合計を計算(マイナスは含めず自然数のみ）
	float total = MinusToPlusf(vector.x) + MinusToPlusf(vector.y);
	//合計から元のVectorを割り割合を計算
	SimpleMath::Vector2 calculationVector = vector / total;
	float setAngle;
	if (vector.y >= 0.0f)
	{
		//下方向ならば真横から真下を向くように
		setAngle = vector.x * 90.0f;
	}
	else
	{
		//上方向に進むのであれば真横から真上を向くように
		setAngle = (MinusToPlusf(vector.y) * 90.0f) + 90.0f;
	}
	if (vector.x >= 0.0f)
	{
		//向かう方向が右方向なら角度をマイナスに
		setAngle = -setAngle;
	}
	else if(setAngle < 0.0f)
	{
		//向かう方向が左方向かつ覚悟がマイナスなのであればplusに
		setAngle = setAngle *-1;
	}
	return setAngle;
}

SimpleMath::Vector2 CF::ChangeAngleToVector(float angle)
{
	//計算の為の定数
	const float CIRCLE = 360;
	const float QUARTER_CIRCLE = 90;
	const float ONE   = 1;
	const float TWO   = 2;
	const float THREE = 3;
	const float FOUR  = 4;

	SimpleMath::Vector2 vector;

	//マイナスを無くした角度
	float newAngle = MinusToPlusf(angle);

	//計算用に角度を360度以内に
	while(newAngle > CIRCLE)
	{
		newAngle -= CIRCLE;
	}

	//角度を90度で割り4以下に(0-90,90-180,180-270,270-360で計算するため）
	float fraction = newAngle / QUARTER_CIRCLE;

	//x+yの合計が１になるように計算
	//0-90の間で計算
	if (fraction <= ONE)
	{
		//下から右へ減らしていくためyを減らしていく
		vector = SimpleMath::Vector2(fraction,ONE- fraction);
	}
	else if(fraction <= TWO)
	{
		//右から上へ減らしていくためxを減らし、上なのでyを-に
		float angleFraction = fraction - ONE;
		vector = SimpleMath::Vector2(ONE - angleFraction, angleFraction * -ONE);
	}
	else if (fraction <= THREE)
	{
		//上から左へ減らしていくためyを減らし、左なのでxを-に、上なのでyをマイナスに
		float angleFraction = fraction - TWO;
		vector = SimpleMath::Vector2(angleFraction * -ONE, (ONE - angleFraction) * -ONE);
	}
	else
	{
		//左から下へ減らしていくためxを減らし、左なのでxを-に
		float angleFraction = fraction - THREE;
		vector = SimpleMath::Vector2((ONE - angleFraction) * -ONE, angleFraction);
	}

	//角度がマイナスの場合x方向を逆に
	if (angle < 0.0f) { vector.x = vector.x * -ONE; }
	return vector;
}

float CF::MinusToPlusf(float minus)
{
	if (minus < 0.0f)
	{
		minus = minus * -1.0f;
	}
	return minus;
}

int CF::MinusToPlus(int minus)
{
	if (minus < 0)
	{
		minus = minus * -1;
	}
	return minus;
}

SimpleMath::Vector2 CF::RectangleCornerAngle(SimpleMath::Vector2 size)
{
	//定数の計算用
	const float ANGLE = 90;
	const float ONE   = 1;

	//サイズの割合を計算
	SimpleMath::Vector2 ratio = SimpleMath::Vector2(size.x / size.y, ONE);

	//割合に応じた角度を計算
	float ratioAngle = ANGLE / (ratio.x + ratio.y);

	//割合に応じた角度をかけて中心点から角への角度を返す
	return ratio * ratioAngle;
}

SimpleMath::Vector2 CF::SquareMovement(float theta, SimpleMath::Vector2 centerPosition, float distance)
{
	//指定された角度を関数用に値を設定
	double angle = theta / 57.295791f;
	SimpleMath::Vector2 differencePosition;
	//座標を参照し位置を移動
	differencePosition.x = centerPosition.x + distance * cos(angle);
	differencePosition.y = centerPosition.y + distance * sin(angle);
	return differencePosition;
}
