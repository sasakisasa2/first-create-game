#include "BossAttack.h"

#pragma region Normal

BossAttack::Normal::Normal():isMoveStart(false), count(0)
{
	//�S�̂̍U����
	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		//�U���񐔂ɉ������ŏ��̒e�̔z��̐�
		int numberCountMax = normalOneShot * attackCount;

		//�e�̌�����
		float shotVectorY = -1.0f;
		//�U���񐔂��������������������肷��
		if (attackCount % 2 == 0) 
		{
			  shotVectorY = 1.0f;
		}

		//���̍U���Ō��e�̐�
		for (int bulletNumber = 0; bulletNumber < NORMAL_ONESHOT_MAX; bulletNumber++)
		{
			isShot[bulletNumber + numberCountMax]= false;

			moveVector[bulletNumber + numberCountMax]
				= ShotVectorSet(shotVectorY, NORMAL_SHOT_BULLET_MAX, NORMAL_SHOT_COUNT, bulletNumber);
		}
	}
}


void BossAttack::Normal::ShotMove()
{	
	// �e�̈ړ�
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{
		if (isShot[bulletNumber]) 
		{
			position[bulletNumber] += moveVector[bulletNumber] * GetNormalShotSpeed() * DXTK->Time.deltaTime;
		}
	}
}

void BossAttack::Normal::ShotPreparation(Vector2 bossPosition)
{
	if (isMoveStart) { return; }
	//���̍U���Ō��e�̐�
	for (int bulletNumber = 0; bulletNumber < NORMAL_ONESHOT_MAX; bulletNumber++)
	{	
			 isShot[bulletNumber+ (NORMAL_ONESHOT_MAX * count)] = true;
			 position[bulletNumber + (NORMAL_ONESHOT_MAX * count)] = bossPosition;

		if (bulletNumber == NORMAL_ONESHOT_MAX - 1) 
		{
			count++;
			isMoveStart = true; 
			break;
		}
	}
}

Vector2 BossAttack::Normal::ShotVectorSet(float shotVectorY,float shotMax,float shotCountMax, float bulletNumber)
{
	Vector2 vector;
	//�S�̐��̔�������e�̔z��̐���������A�e�̕��������߂�
	float place = bulletNumber / (((shotMax / shotCountMax) / 2.0f)-1);
	vector.x = -1.0f + place;
	if (place >= 1.0f)
	{
		place= 2.0f - place;
	}
	vector.y = place * shotVectorY;
	return vector;
}
#pragma endregion

#pragma region AimShot

BossAttack::AimShot::AimShot()
{
	for (int bulletNumber = 0; bulletNumber < AIMSHOT_COUNT; bulletNumber++)
	{
		isShotMove[bulletNumber]  = 0;
		isPlaceMove[bulletNumber] = 0;
	}
}

void BossAttack::AimShot::ShotReserve()
{
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{
		isShotMove[attackCount]  = false;

		isPlaceMove[attackCount] = false;
	}
}

Vector2 BossAttack::AimShot::SetAttackReserve(int attackCount, int bulletNumber)
{
	Vector2 setPosition;
	setPosition = Vector2(
		GetAimShotShotPlace() * ((bulletNumber - AIMSHOT_ONEATTACK_BULLET_MAX * attackCount) + 1.0f) + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	//���@�_���e
	Vector2 setPositionMove;
	//�w�莞�ԑ���
	bool isTimer = shotTimer.TimeMeasurement(GetToShotTime());
	//�U����
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{		
		//�e��
		for (int bulletNumber = AIMSHOT_ONEATTACK_BULLET_MAX * attackCount; bulletNumber < AIMSHOT_ONEATTACK_BULLET_MAX * (attackCount + 1.0f); bulletNumber++)
		{
			//�U������
			if (!isPlaceMove[attackCount])
			{
				position[bulletNumber] = bossPosition;

				if (bulletNumber == AIMSHOT_ONEATTACK_BULLET_MAX - 1)
					isPlaceMove[attackCount] = true;
			}
			//�U���O�̈ʒu�ֈړ�
			if (isPlaceMove[attackCount] && !isShotMove[attackCount])
			{
				//�U�������ʒu�̕����x�N�g��
				setPositionMove = CF::DistanceCount(SetAttackReserve(attackCount, bulletNumber), position[bulletNumber]);
				//�U�������ʒu�ֈړ�
				position[bulletNumber] += setPositionMove * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
			}
			if (!isShotMove[attackCount])
			{
				//�Ώۂւ̃x�N�g�����v�Z
				shotVector[bulletNumber] = CF::DistanceCount(playerPosition, position[bulletNumber]);
			}
			if (isShotMove[attackCount])
			{
				//�Ώۂ֍U��
				position[bulletNumber] += shotVector[bulletNumber] * GetAimShotShotSpeed() * DXTK->Time.deltaTime;
			}
		}
		if (isTimer)
		{
			if (attackCount == AIMSHOT_COUNT - 1.0f)
			{
				//�e�̎c��̔���
				isShotMove[attackCount] = true;
				shotTimer.TimerReSet();
			}
			else
			{
				//�e�̔����̔��˂Ɠ����Ɏ��̒e�̏���
				isShotMove[attackCount] = true;
				isPlaceMove[attackCount + 1] = true;
				shotTimer.TimerReSet();
			}
		}
		else 
		{
			break;
		}
	}
	//��ʊO�ɏo���珉����
	if (CF::PositionRengeOver(position[AIMSHOT_BULLET_MAX - 1],
		Vector2(-sprite.size.x, DXTK->Screen.Width + sprite.size.x),
		Vector2(-sprite.size.y, DXTK->Screen.Height + sprite.size.y)))
	{
		for (int bulletNumber = 0; bulletNumber < AIMSHOT_COUNT; bulletNumber++)
		{
			isShotMove[bulletNumber]  = false;
			isPlaceMove[bulletNumber] = false;
		}
	}
}

#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(false), isPositionUpdate(true),
	moveVector() , oldPlayerPosition(),
	volume(1)
{ } 

void BossAttack::Induction::InductionStart(Vector2 startPosition)
{
	volume			 = 1;
	isMove			 = true;
	isPositionUpdate = true;
	position[0]		 = startPosition;
	largeTimer.TimerReSet();
}

void BossAttack::Induction::Update(Vector2 playerPosition) 
{
	//�T�C�Y�̍X�V
	sprite.size.x = GetInductionSpriteSize().x * volume;
	sprite.size.y = GetInductionSpriteSize().y * volume;
	
	bool isLarge = largeTimer.TimeMeasurement(GetVolumeLargeTime());

	//�������߂��Ȃ����ۂɒǔ��n�_���X�V
	if (CF::Distance(oldPlayerPosition,position[0]) <= 1) 
	{
		isPositionUpdate = true;
	}

	//�ǔ��n�_�̍X�V
	if (isPositionUpdate || volume == GetMaxVolume() && isLarge)
	{
		oldPlayerPosition = playerPosition;
		isPositionUpdate = false;
	}

	//�ő�T�C�Y����莞�Ԍ�ŏI�U��
	if (volume == GetMaxVolume() && isLarge)
	{
		isMove = false;
		position[0] += CF::DistanceCount(oldPlayerPosition, position[0]) *
					   GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		if (CF::PositionRengeOver (position[0],
				 Vector2(-sprite.size.x, DXTK->Screen.Width  + sprite.size.x),
				 Vector2(-sprite.size.y, DXTK->Screen.Height + sprite.size.y)))
		{
			largeTimer.TimerReSet();
			volume = 1;
		}
	}

	//�ǐՈړ�
	if (isMove) 
	{
		position[0] += CF::DistanceCount(oldPlayerPosition, position[0])*
					   GetInductionMoveSpeed() * volume * DXTK->Time.deltaTime;
	}
	//�傫���ω�
	if (volume != GetMaxVolume() && isLarge)
	{
		volume++;
		largeTimer.TimerReSet();
	}
}
#pragma endregion

#pragma region Frame

BossAttack::Frame::Frame(): move()
{
	screenMin = Vector2(0.0f + GetFrameSpriteSize().x / 2.0f, 0.0f + GetFrameSpriteSize().y / 2.0f);
	screenMax = Vector2(DXTK->Screen.Width - GetFrameSpriteSize().x/2.0f, DXTK->Screen.Height - GetFrameSpriteSize().y/2.0f);
}

void BossAttack::Frame::PositionSet()
{	
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{	
#pragma region �ŏ��̈ʒu�̐ݒ�

		float   eighthPlusQuarter = GetFrameEighth() + GetFrameQuarter();
		Vector2 setPosition;
		//�e�ő吔��8����1����ʍ��[�ɐݒu
		if (bulletNumber <= GetFrameEighth())
		{
			setPosition 
				= Vector2(screenMin.x,
		                  (screenMax.y / GetFrameEighth()) * bulletNumber);
		}
		//�e�ő吔��8����1����ʉE�[�ɐݒu
		else if (bulletNumber <= GetFrameQuarter())
		{
			setPosition 
				= Vector2(screenMax.x,
				         (screenMax.y / GetFrameEighth()) * (bulletNumber - GetFrameEighth()));
		}
		//�c��̒e���̔�������ʏ㕔�ɐݒu
		else if (bulletNumber <= (GetFrameQuarter() * 3.0f) - GetFrameEighth())
		{
			setPosition
				= Vector2((screenMax.x / eighthPlusQuarter) * (bulletNumber - GetFrameQuarter()),
					      screenMin.y);
		}
		//�c�����ʉ����ɐݒu
		else
		{
			float frameNumber = (GetFrameQuarter() * 3.0f) - GetFrameEighth();
			setPosition
				= Vector2(screenMax.x / eighthPlusQuarter *(bulletNumber - frameNumber),
					      screenMax.y);
		}

		position[bulletNumber] = setPosition;

#pragma endregion
		//�������������w��
		VectorSet(bulletNumber);
	}
}

void BossAttack::Frame::Update()
{
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{
		//�w������ړ�
		position[bulletNumber] += move[bulletNumber] * GetFrameMoveSpeed() * DXTK->Time.deltaTime;
		//�؂�Ԃ�
		OutRenge(bulletNumber);
	}
}

void BossAttack::Frame::OutRenge(int number)
{
	bool rengeOver = CF::PositionRengeOver(position[number], screenMin, screenMax);
	//�w��͈͊O�ɏo�Ă��Ȃ��ꍇ�߂�
	if (!rengeOver)
	{
		return;
	}	

    //�i��ł����������玞�v���̕����ɐ؂�Ԃ�
	float reStartMove;
	
	//��E�������獶������
	if (move[number].y == -1.0f)
	{
		//�w��͈͊O����o�����w������ɉ����A�t���[�����K�^�K�^�ɂȂ�Ȃ��悤��
		move[number] = Vector2(-1.0f,0.0f);
		reStartMove	     = position[number].y - screenMin.y;
		position[number] = Vector2(screenMax.x + reStartMove , screenMin.y);
	}
	//�㍶�[���牺������
	else if (move[number].x == -1.0f)
	{
		move[number] = Vector2(0.0f, 1.0f);
		reStartMove	     = screenMin.x - position[number].x;
		position[number] = Vector2(screenMin.x, screenMin.y + reStartMove);
	}
	//�����[����E������
	else if (move[number].y == 1.0f)
	{
		move[number]	 = Vector2(1.0f, 0.0f);
		reStartMove	     = screenMax.y - position[number].y;
		position[number] = Vector2(screenMin.x - reStartMove, screenMax.y);
	}
	//���E�[����������
	else if (move[number].x == 1.0f)
	{
		move[number] = Vector2(0.0f, -1.0f);
		reStartMove	     = screenMax.x - position[number].x;
		position[number] = Vector2(screenMax.x, screenMax.y + reStartMove);
	}
}

void BossAttack::Frame::VectorSet(int number)
{
	//�㕔�ɋ���Ƃ���������
	if (position[number].x <= screenMax.x && position[number].y <= screenMin.y)
	{
		move[number] = Vector2(-1.0f, 0.0f);
	}
	//�����ɋ���Ƃ���������
	if (position[number].x <= screenMin.x && position[number].y >= screenMin.y)
	{
		move[number] = Vector2(0.0f, 1.0f);
	}
	//�����ɋ���Ƃ��E������
	if (position[number].x >= screenMin.x && position[number].y >= screenMax.y)
	{
		move[number] = Vector2(1.0f, 0.0f);
	}
	//�E���ɋ���Ƃ��������
	if (position[number].x >= screenMax.x && position[number].y <= screenMax.y)
	{
		move[number] = Vector2(0.0f, -1.0f);
	}
}
#pragma endregion

