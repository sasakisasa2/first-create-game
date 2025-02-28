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

CF::Random::Random()
{
	std::random_device rd;
	random = std::mt19937(rd());
}

float CF::Random::SetRandomf(SimpleMath::Vector2 randamRange)
{
	std::uniform_real_distribution<float> other;
	other = std::uniform_real_distribution<float>(randamRange.x, randamRange.y);
	return other(random);
}

int CF::Random::SetRandom(SimpleMath::Vector2 randamRange)
{
	std::uniform_real_distribution<float> other;
	other = std::uniform_real_distribution<float>(randamRange.x, randamRange.y);
	return (int)other(random);
}

SimpleMath::Vector2 CF::DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo)
{
	float distance = sqrt((positionOne.x - positionTwo.x) *
					      (positionOne.x - positionTwo.x) +
						  (positionOne.y - positionTwo.y) *
						  (positionOne.y - positionTwo.y));
	SimpleMath::Vector2 vector = (positionOne - positionTwo) / distance;
	return vector;
}

float CF::Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo)
{
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
	const float CIRCLE = 360;
	const float QUARTER_CIRCLE = 90;
	SimpleMath::Vector2 vector;
	float newAngle = MinusToPlusf(angle);
	while(newAngle > CIRCLE)
	{
		newAngle -= CIRCLE;
	}
	float fraction = newAngle / QUARTER_CIRCLE;
	if (fraction <= 1.0f)
	{
		vector = SimpleMath::Vector2(fraction,1.0f- fraction);
	}
	else if(fraction <= 2.0f)
	{
		float angleFraction = fraction - 1.0f;
		vector = SimpleMath::Vector2(1.0f - angleFraction, angleFraction * -1.0f);
	}
	else if (fraction <= 3.0f)
	{
		float angleFraction = fraction - 2.0f;
		vector = SimpleMath::Vector2(angleFraction * -1.0f, 1.0f - angleFraction);
	}
	else
	{
		float angleFraction = fraction - 3.0f;
		vector = SimpleMath::Vector2(1.0f - angleFraction, angleFraction);
	}
	if (angle < 0.0f) { vector.x = vector.x * -1; }
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
