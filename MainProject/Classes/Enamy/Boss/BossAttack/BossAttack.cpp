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
		color[bulletNumber] = Vector4(0.0f,0,0,0);
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
		isShotMove  = false;
		isPlaceMove = false;
	}
}

void BossAttack::AimShot::ShotReserve()
{
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{
		isShotMove  = false;
		isPlaceMove = false;
	}
}

Vector2 BossAttack::AimShot::SetAttackReserve(int bulletNumber)
{
	Vector2 setPosition;
	setPosition = Vector2(
		GetAimShotShotPlace() * (bulletNumber + 1.0f)  + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	//�w�莞�ԑ���
	if (shotTimer.TimeMeasurement(GetToShotTime()))
	{
			//if (attackCount == AIMSHOT_COUNT - 1.0f)
			//{
				//�e�̎c��̔���
				isShotMove = true;
				shotTimer.TimerReSet();
			//}
			//else
			//{
				////�e�̔����̔��˂Ɠ����Ɏ��̒e�̏���
				//isShotMove = true;
				//isPlaceMove[attackCount + 1] = true;
				//shotTimer.TimerReSet();
			//}
	}

	//�U�����̒e��
	for (int bulletNumber = 0; bulletNumber < AIMSHOT_ONEATTACK_BULLET_MAX; bulletNumber++)
	{
		//�U������
		if (!isPlaceMove)
		{
			position[bulletNumber] = bossPosition;

			if (bulletNumber == AIMSHOT_ONEATTACK_BULLET_MAX-1)
				isPlaceMove = true;
		}
		//�U���O�̈ʒu�ֈړ�
		if (isPlaceMove && !isShotMove)
		{
			//�U�������ʒu�̕����x�N�g��
			Vector2 setMoveVector = CF::DistanceCount(SetAttackReserve(bulletNumber), position[bulletNumber]);
			//�U�������ʒu�ֈړ�
			position[bulletNumber] += setMoveVector * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
		}
		if (!isShotMove)
		{
			//�Ώۂւ̕������v�Z
			shotVector[bulletNumber] = CF::DistanceCount(playerPosition, position[bulletNumber]);
		}
		if (isShotMove)
		{
			//�Ώۂ֍U��
			position[bulletNumber] += shotVector[bulletNumber] * GetAimShotShotSpeed() * DXTK->Time.deltaTime;
		}
	}
	//��ʊO�ɏo���珉����
	if (CF::PositionRengeOver(position[AIMSHOT_BULLET_MAX - 1],
		Vector2((float)sprite.size.x*-1.0f, (float)DXTK->Screen.Width  + (float)sprite.size.x),
		Vector2((float)sprite.size.y*-1.0f, (float)DXTK->Screen.Height + (float)sprite.size.y)))
	{

		isShotMove  = false;
		isPlaceMove = false;
	}
}
#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(true), isPositionUpdate(true),
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

	if (volume != GetMaxVolume())
	{	
		//���˂܂ŕ������v�Z
		moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);

		//�ǐՈړ�	
		if (isMove)
		{
			position[0] += moveVector * GetInductionMoveSpeed() * volume * DXTK->Time.deltaTime;
		}
		//�������߂��Ȃ����ۂɒǔ��n�_���X�V
		if (CF::Distance(oldPlayerPosition, position[0]) <= 1)
		{
			isPositionUpdate = true;
		}

		//�w�莞�ԂɒB���Ă���ꍇ�T�C�Y��傫������
		if (largeTimer.TimeMeasurement(GetVolumeLargeTime()))
		{
			//�T�C�Y�̊g��
			volume++;
			largeTimer.TimerReSet();
		}
	}

	//�ǔ��n�_�̍X�V
	if (isPositionUpdate)
	{
		oldPlayerPosition = playerPosition;
		isPositionUpdate = false;
	}


	//�ő�T�C�Y����莞�Ԍ�ŏI�U��
	if (volume == GetMaxVolume())
	{
		//��~����
		if (attackCoolTimer.TimeMeasurement(3))
		{
			//���X�g�A�^�b�N
			position[0] += moveVector * GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		}	
		else
		{
			isPositionUpdate = true;
			//���˂܂ŕ������v�Z
			moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);
		}

		if (CF::PositionRengeOver(position[0],
			Vector2((float)sprite.size.x*-1.0f, (float)DXTK->Screen.Width  + (float)sprite.size.x),
			Vector2((float)sprite.size.y*-1.0f, (float)DXTK->Screen.Height + (float)sprite.size.y))
			)	
		{
			attackCoolTimer.TimerReSet();
			largeTimer.TimerReSet();
			volume = 1;
		}
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
		color[bulletNumber] = Vector4(0.0f, 0, 0, 0);
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

