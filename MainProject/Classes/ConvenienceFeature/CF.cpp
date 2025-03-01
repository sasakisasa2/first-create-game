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
	//�����̌v��	
	float distance = sqrt((positionOne.x - positionTwo.x) *
					      (positionOne.x - positionTwo.x) +
						  (positionOne.y - positionTwo.y) *
						  (positionOne.y - positionTwo.y));
	SimpleMath::Vector2 vector = (positionOne - positionTwo) / distance;
	return vector;
}

float CF::Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo)
{
	//�����̌v��	
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

	//�x�N�g����X��Y�̍��v���v�Z(�}�C�i�X�͊܂߂����R���̂݁j
	float total = MinusToPlusf(vector.x) + MinusToPlusf(vector.y);
	//���v���猳��Vector�����芄�����v�Z
	SimpleMath::Vector2 calculationVector = vector / total;
	float setAngle;
	if (vector.y >= 0.0f)
	{
		//�������Ȃ�ΐ^������^���������悤��
		setAngle = vector.x * 90.0f;
	}
	else
	{
		//������ɐi�ނ̂ł���ΐ^������^��������悤��
		setAngle = (MinusToPlusf(vector.y) * 90.0f) + 90.0f;
	}
	if (vector.x >= 0.0f)
	{
		//�������������E�����Ȃ�p�x���}�C�i�X��
		setAngle = -setAngle;
	}
	else if(setAngle < 0.0f)
	{
		//���������������������o�傪�}�C�i�X�Ȃ̂ł����plus��
		setAngle = setAngle *-1;
	}
	return setAngle;
}

SimpleMath::Vector2 CF::ChangeAngleToVector(float angle)
{
	//�v�Z�ׂ̈̒萔
	const float CIRCLE = 360;
	const float QUARTER_CIRCLE = 90;
	const float ONE   = 1;
	const float TWO   = 2;
	const float THREE = 3;
	const float FOUR  = 4;

	SimpleMath::Vector2 vector;

	//�}�C�i�X�𖳂������p�x
	float newAngle = MinusToPlusf(angle);

	//�v�Z�p�Ɋp�x��360�x�ȓ���
	while(newAngle > CIRCLE)
	{
		newAngle -= CIRCLE;
	}

	//�p�x��90�x�Ŋ���4�ȉ���(0-90,90-180,180-270,270-360�Ōv�Z���邽�߁j
	float fraction = newAngle / QUARTER_CIRCLE;

	//x+y�̍��v���P�ɂȂ�悤�Ɍv�Z
	//0-90�̊ԂŌv�Z
	if (fraction <= ONE)
	{
		//������E�֌��炵�Ă�������y�����炵�Ă���
		vector = SimpleMath::Vector2(fraction,ONE- fraction);
	}
	else if(fraction <= TWO)
	{
		//�E�����֌��炵�Ă�������x�����炵�A��Ȃ̂�y��-��
		float angleFraction = fraction - ONE;
		vector = SimpleMath::Vector2(ONE - angleFraction, angleFraction * -ONE);
	}
	else if (fraction <= THREE)
	{
		//�ォ�獶�֌��炵�Ă�������y�����炵�A���Ȃ̂�x��-�ɁA��Ȃ̂�y���}�C�i�X��
		float angleFraction = fraction - TWO;
		vector = SimpleMath::Vector2(angleFraction * -ONE, (ONE - angleFraction) * -ONE);
	}
	else
	{
		//�����牺�֌��炵�Ă�������x�����炵�A���Ȃ̂�x��-��
		float angleFraction = fraction - THREE;
		vector = SimpleMath::Vector2((ONE - angleFraction) * -ONE, angleFraction);
	}

	//�p�x���}�C�i�X�̏ꍇx�������t��
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
	//�萔�̌v�Z�p
	const float ANGLE = 90;
	const float ONE   = 1;

	//�T�C�Y�̊������v�Z
	SimpleMath::Vector2 ratio = SimpleMath::Vector2(size.x / size.y, ONE);

	//�����ɉ������p�x���v�Z
	float ratioAngle = ANGLE / (ratio.x + ratio.y);

	//�����ɉ������p�x�������Ē��S�_����p�ւ̊p�x��Ԃ�
	return ratio * ratioAngle;
}

SimpleMath::Vector2 CF::SquareMovement(float theta, SimpleMath::Vector2 centerPosition, float distance)
{
	//�w�肳�ꂽ�p�x���֐��p�ɒl��ݒ�
	double angle = theta / 57.295791f;
	SimpleMath::Vector2 differencePosition;
	//���W���Q�Ƃ��ʒu���ړ�
	differencePosition.x = centerPosition.x + distance * cos(angle);
	differencePosition.y = centerPosition.y + distance * sin(angle);
	return differencePosition;
}
