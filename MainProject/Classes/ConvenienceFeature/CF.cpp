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