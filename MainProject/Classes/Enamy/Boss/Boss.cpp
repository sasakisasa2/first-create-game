#include "Boss.h"

void Boss::Load(DirectXTK::Sprite& bossSprite, DirectXTK::Sprite& atttackSprite, DirectXTK::Sprite& hpSprite, DirectXTK::Sprite& hpBackGroundSprite)
{
	move.Load(bossSprite);

	frameAttack.Load(atttackSprite);
	
	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		normalAttack[attackCount].Load(atttackSprite);
	}

	inductionAttack.Load(atttackSprite);


	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].Load(atttackSprite);
	}
}

void Boss::Initialize()
{
	move.SpriteSizeSet(GetBossSpriteSize());

	frameAttack.SpriteSizeSet(GetFrameSpriteSize());

	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		normalAttack[attackCount].SpriteSizeSet(GetNormalSpriteSize());
	}

	inductionAttack.SpriteSizeSet(GetInductionSpriteSize());


	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].SpriteSizeSet(GetAimShotSpriteSize());
	}

	frameAttack.PositionSet();
	normalAttack[0].ShotPreparation(move.GetPosition());
	isNormalAttackShot[0] = true;
}

void Boss::Update(Vector2 playerPosition)
{
	float bossVector = 0.0f;

	switch (attackMode)
	{
	case 1:
		frameAttack.Update();

		//�����蔻��̏��̐ݒ�
		for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++)
		{
			frameCollisionInfo[bulletNumber].
				SetSquareCorner(
					frameAttack.GetPosition(bulletNumber),
					GetFrameSpriteSize(),
					frameAttack.GetMoveVector(bulletNumber),
					frameAttack.GetAngle(bulletNumber),
					GetFrameMoveSpeed()
				);
		}
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			if (isNormalAttackShot[attackCount])
			{
				//�U�������̐ݒ�
				float vector = 1.0f;
				if (attackCount % 2 == 1) { vector = -1.0f; }

				//�U���J�n
				normalAttack[attackCount].ShotMove(vector);

				for (int bulletCount = 0; bulletCount < NORMAL_SHOT_BULLET_MAX; bulletCount++)
				{
					normalCollisionInfo[bulletCount * attackCount].
						SetSquareCorner(
						normalAttack[attackCount].GetPosition(bulletCount),
						GetNormalSpriteSize(),
						normalAttack[attackCount].GetMoveVector(bulletCount),
						normalAttack[attackCount].GetAngle(bulletCount),
						GetNormalShotSpeed()
						);
				}
			}
		}

		//�ŏI�U�����I���܂ō쓮
		if (normalAttackCount != NORMAL_SHOT_COUNT)
		{
			//�������ւ̈ړ�
			bossVector = -1.0f;
			//�ォ�牺�ւ̈ړ�
			if (normalAttackCount % 2 == 0)
				bossVector = 1.0f; 

				move.Update(-1.0f);

			if (move.GetIsMoveEnd())
			{
				normalAttackCount++;                         //�U���񐔂̃J�E���g
				move.SetIsMoveEnd(false);                    //�ړ��̍ĊJ
				isNormalAttackShot[normalAttackCount] = true;//���̍U���̍ĊJ
				normalAttack[normalAttackCount].ShotPreparation(move.GetPosition());//�U���̏���
			}
		}
		else
		{
			frameAttack.SetIsVectorSwitch(true);
			if (normalAttack[normalAttackCount-1].GetIsShotEnd()&&frameAttack.GetIsShotEnd())
			{
				normalAttackCount = 0;
				inductionAttack.InductionStart(move.GetPosition());
				attackMode++;
			}
		}
		break;
	case 2:
		inductionAttack.Update(playerPosition);

		float speed;
		if (inductionAttack.GetIsLastAttack())
		{
			speed = GetInductionLastAttackSpeed();
		}
		else
		{
			speed = GetInductionMoveSpeed() * inductionAttack.GetVolume();
		}

		inductionCollisionInfo.
			SetSquareCorner(
				inductionAttack.GetPosition(),
				GetInductionSpriteSize() * inductionAttack.GetVolume(),
				inductionAttack.GetMoveVector(),
				inductionAttack.GetAngle(),
				speed
			);

		if (inductionAttack.GetIsShotEnd())
		{
			attackMode++;
		}
		break;
	case 3:
	for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
	{
		if (aimShotTimer[attackCount].TimeMeasurement(3))
		{
			aimShotAttack[attackCount].Update(move.GetPosition(), playerPosition);
			if (attackCount == aimShotMoveCount-1&& aimShotMoveCount != AIMSHOT_COUNT) { aimShotMoveCount++; }
		}

		for (int bulletCount = AIMSHOT_BULLET_MAX * attackCount; bulletCount < AIMSHOT_BULLET_MAX * (attackCount + 1); bulletCount++)
		{
			float speed = GetAimShotSettingSpeed();
			Vector2 vector = aimShotAttack[attackCount].GetReserveVector(bulletCount - (AIMSHOT_BULLET_MAX * attackCount));

			if (aimShotAttack[attackCount].GetIsShotMove()) 
			{
				speed = GetAimShotSpeed();
				vector = aimShotAttack[attackCount].GetShotVector(bulletCount - (AIMSHOT_BULLET_MAX * attackCount));
			}
			aimShotCollisionInfo[bulletCount].
				SetSquareCorner(
					aimShotAttack[attackCount].GetPosition(bulletCount - (AIMSHOT_BULLET_MAX * attackCount)),
					GetAimShotSpriteSize(),
					vector,
					aimShotAttack[attackCount].GetAngle(bulletCount - (AIMSHOT_BULLET_MAX * attackCount)),
					speed
				);
		}
	}
	if (aimShotAttack[aimShotMoveCount-1].GetIsShotEnd())
	{
		//�G�C���V���b�g�̏�����
		for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
		{
			aimShotAttack[attackCount].ShotReserve();
		}
		aimShotMoveCount = 1;	
		
		//�m�[�}���U���̏�����
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			normalAttack[attackCount].ShotPreparation(move.GetPosition());
		}
		//�U���J�n�ׂ̈̏���
		isNormalAttackShot[0] = true;
		frameAttack.PositionSet();

		attackMode = 1;
	}
	break;
	default:
		attackMode = 1;
	};

	bossCollisionInfo.
		SetSquareCorner(
			move.GetPosition(),
			GetBossSpriteSize(),
			Vector2(0.0f,bossVector),
			0.0f,
			GetMoveSpeed()
		);
}

void Boss::Render(DirectX::SpriteBatch* SpriteBatch)
{	
	move.Render(SpriteBatch, 0);
	switch (attackMode)
	{
	case 1:
		//�e�̕`��
		for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++)
		{
			frameAttack.Render(SpriteBatch, bulletNumber);
		}
		//�U���񐔕`��
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			//�e�̕`��
			for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
			{
				normalAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	case 2:
		//�e�̕`��
		inductionAttack.Render(SpriteBatch, 0);
		break;
	case 3:
		for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
		{
			//�e�̕`��
			for (int bulletNumber = 0; bulletNumber < AIMSHOT_BULLET_MAX; bulletNumber++)
			{
				aimShotAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	};
}
