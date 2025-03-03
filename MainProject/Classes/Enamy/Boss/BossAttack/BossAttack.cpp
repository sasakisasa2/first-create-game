#include "BossAttack.h"

#pragma region Normal

void BossAttack::Normal::ShotMove(float moveVector)
{	
	int shotEndCount = 0;

	// �e�̈ړ�
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{
		//�ݒ�����ֈړ�
		position[bulletNumber] += ShotVectorSet(moveVector, NORMAL_SHOT_BULLET_MAX, bulletNumber) * 
								  GetNormalShotSpeed() * DXTK->Time.deltaTime;
		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			shotEndCount++;
			if (shotEndCount == NORMAL_SHOT_BULLET_MAX) { SetIsShotEnd(true); }
		}

	}
}

void BossAttack::Normal::ShotPreparation(Vector2 startPosition)
{
	//���̍U���Ō��e�̐�
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{	
		position[bulletNumber] = startPosition;
	}
}

Vector2 BossAttack::Normal::ShotVectorSet(float shotVectorY,float shotMax, float bulletNumber)
{
	Vector2 vector;
	//�S�̐��̔�������e�̔z��̐���������A�e�̕��������߂�
	float place = bulletNumber / ((shotMax  / 2.0f));
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

void BossAttack::AimShot::ShotReserve()
{
	isShotMove  = false;
	isPlaceMove = false;
	SetIsShotEnd(false);
}

Vector2 BossAttack::AimShot::SetAttackReserve(int bulletNumber)
{
	Vector2 setPosition;
	//�����ʒu�̌v�Z
	//x.(�e���m�̊Ԋu*(�����ڂ�)+�w��͈͂܂ł̋���),y.�e�̍���
	setPosition = Vector2(
		GetAimShotShotPlace() * bulletNumber + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	if (GetIsShotEnd()) { return; }
	
	//�e�̔��˂܂ł̎��Ԃ𑪂�
	if (!isShotMove)
	{
		if (shotTimer.TimeMeasurement(GetToShotTime()))
		{
			//�e�̔���
			isShotMove = true;
			shotTimer.TimerReSet();
		}
	}

	int endCount = 0;//�U�����I��������ǂ������肷�邽�߂̕ϐ�
	
	//�U���̒e������
	for (int bulletNumber = 0; bulletNumber < AIMSHOT_BULLET_MAX; bulletNumber++)
	{
		//�U������
		if (!isPlaceMove)
		{
			position[bulletNumber] = bossPosition;

			if (bulletNumber == AIMSHOT_BULLET_MAX-1)
				isPlaceMove = true;
		}
		//�U���O�̈ʒu�ֈړ�
		if (isPlaceMove && !isShotMove)
		{
			//�U�������ʒu�̕����x�N�g��
			reserveVector[bulletNumber] = CF::DistanceCount(SetAttackReserve(bulletNumber), position[bulletNumber]);
			//�U�������ʒu�ֈړ�
			position[bulletNumber] += reserveVector[bulletNumber] * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
			angle[bulletNumber] = CF::ChangeVectorToAngle(reserveVector[bulletNumber], angle[bulletNumber]);
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
			angle[bulletNumber] = CF::ChangeVectorToAngle(shotVector[bulletNumber], angle[bulletNumber]);
		}
		//�e���͈͊O�ɂ��邩�ǂ����̊m�F
		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			endCount++;
			//�U���S�Ă��͈͊O�������ꍇ
			if (endCount == AIMSHOT_BULLET_MAX) { SetIsShotEnd(true); }
		}
	}
}
#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(true), isPositionUpdate(true),
	isLastAttack(false),
	moveVector() , oldPlayerPosition(),
	volume(1)
{ } 

void BossAttack::Induction::InductionStart(Vector2 startPosition)
{
	volume			 = 1;
	isMove			 = true;
	isPositionUpdate = true;
	position[0]		 = startPosition;
	SetIsShotEnd(false);
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
		angle[0] = CF::ChangeVectorToAngle(CF::DistanceCount(oldPlayerPosition,position[0]), angle[0]);
		isPositionUpdate = false;
	}


	//�ő�T�C�Y����莞�Ԍ�ŏI�U��
	if (volume == GetMaxVolume())
	{
		//��~����
		if (attackCoolTimer.TimeMeasurement(GetInductionAttackStopTime()))
		{
			isLastAttack = true;
			//���X�g�A�^�b�N
			position[0] += moveVector * GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		}	
		else
		{
			isPositionUpdate = true;
			//���˂܂ŕ������v�Z
			moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);
		}

		if (CF::PositionRangeOver(position[0],XRange(), YRange()))
		{
			volume = 1;
			isMove = false;
			isPositionUpdate = false;
			SetIsShotEnd(true);
			attackCoolTimer.TimerReSet();
			largeTimer.TimerReSet();
		}
	}
}
#pragma endregion

#pragma region Frame

BossAttack::Frame::Frame(): move(), isVectorSwitch(false)
{
	xRange = Vector2((float)GetFrameSpriteSize().x / 2.0f, (float)DXTK->Screen.Width  - (float)GetFrameSpriteSize().x / 2.0f);
	yRange = Vector2((float)GetFrameSpriteSize().y / 2.0f, (float)DXTK->Screen.Height - (float)GetFrameSpriteSize().y / 2.0f);
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
				= Vector2(xRange.x,
		                  (yRange.y / GetFrameEighth()) * bulletNumber);
		}
		//�e�ő吔��8����1����ʉE�[�ɐݒu
		else if (bulletNumber <= GetFrameQuarter())
		{
			setPosition 
				= Vector2(xRange.y,
				         (yRange.y / GetFrameEighth()) * (bulletNumber - GetFrameEighth()));
		}
		//�c��̒e���̔�������ʏ㕔�ɐݒu
		else if (bulletNumber <= (GetFrameQuarter() * 3.0f) - GetFrameEighth())
		{
			setPosition
				= Vector2((xRange.y / eighthPlusQuarter) * (bulletNumber - GetFrameQuarter()),
					       yRange.x);
		}
		//�c�����ʉ����ɐݒu
		else
		{
			float frameNumber = (GetFrameQuarter() * 3.0f) - GetFrameEighth();
			setPosition
				= Vector2(xRange.y / eighthPlusQuarter *(bulletNumber - frameNumber),
					      yRange.y);
		}

		position[bulletNumber] = setPosition;

#pragma endregion
		//�������������w��
		VectorSet(bulletNumber);
	}
}

void BossAttack::Frame::Update()
{
	int moveEndCount = 0;
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{
		//�w������ړ�
		position[bulletNumber] += move[bulletNumber] * GetFrameMoveSpeed() * DXTK->Time.deltaTime;
		angle[bulletNumber]     = CF::ChangeVectorToAngle(move[bulletNumber], angle[bulletNumber]);

		//�؂�Ԃ�	
		if(!isVectorSwitch)
		OutRange(bulletNumber);

		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			moveEndCount++;
			if (moveEndCount == FRAME_BULLET_MAX) { SetIsShotEnd(true); }
		}
	}
}

void BossAttack::Frame::OutRange(int number)
{
	bool rengeOver = CF::PositionRangeOver(position[number], xRange, yRange);
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
		reStartMove	     = position[number].y - yRange.x;
		position[number] = Vector2(xRange.y + reStartMove, yRange.x);
	}
	//�㍶�[���牺������
	else if (move[number].x == -1.0f)
	{
		move[number] = Vector2(0.0f, 1.0f);
		reStartMove	     = xRange.x - position[number].x;
		position[number] = Vector2(xRange.x, yRange.x + reStartMove);
	}
	//�����[����E������
	else if (move[number].y == 1.0f)
	{
		move[number]	 = Vector2(1.0f, 0.0f);
		reStartMove	     = position[number].y - yRange.y;
		position[number] = Vector2(xRange.x + reStartMove, yRange.y);
	}
	//���E�[����������
	else if (move[number].x == 1.0f)
	{
		move[number] = Vector2(0.0f, -1.0f);
		reStartMove	     = xRange.y - position[number].x;
		position[number] = Vector2(xRange.y, yRange.y + reStartMove);
	}
}

void BossAttack::Frame::VectorSet(int number)
{
	//�㕔�ɋ���Ƃ���������
	if (position[number].x <= xRange.y && position[number].y <= yRange.x)
	{
		move[number] = Vector2(-1.0f, 0.0f);
	}
	//�����ɋ���Ƃ���������
	if (position[number].x <= xRange.x && position[number].y >= yRange.x)
	{
		move[number] = Vector2(0.0f, 1.0f);
	}
	//�����ɋ���Ƃ��E������
	if (position[number].x >= xRange.x && position[number].y >= yRange.y)
	{
		move[number] = Vector2(1.0f, 0.0f);
	}
	//�E���ɋ���Ƃ��������
	if (position[number].x >= xRange.y && position[number].y <= yRange.y)
	{
		move[number] = Vector2(0.0f, -1.0f);
	}
}
#pragma endregion

