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
	switch (attackMode)
	{
	case 1:
		frameAttack.Update();

		//“–‚½‚è”»’è‚Ìî•ñ‚ÌÝ’è
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
				//UŒ‚•ûŒü‚ÌÝ’è
				float vector = 1.0f;
				if (attackCount % 2 == 1) { vector = -1.0f; }

				//UŒ‚ŠJŽn
				normalAttack[attackCount].ShotMove(vector);
			}
		}

		//ÅIUŒ‚‚ªI‚í‚é‚Ü‚Åì“®
		if (normalAttackCount != NORMAL_SHOT_COUNT)
		{
			//ã‚©‚ç‰º‚Ö‚ÌˆÚ“®
			if (normalAttackCount % 2 == 0)
			{
				move.Update(1.0f);
			}
			//‰º‚©‚çã‚Ö‚ÌˆÚ“®
			else
			{
				move.Update(-1.0f);
			}
			if (move.GetIsMoveEnd())
			{
				normalAttackCount++;                         //UŒ‚‰ñ”‚ÌƒJƒEƒ“ƒg
				move.SetIsMoveEnd(false);                    //ˆÚ“®‚ÌÄŠJ
				isNormalAttackShot[normalAttackCount] = true;//ŽŸ‚ÌUŒ‚‚ÌÄŠJ
				normalAttack[normalAttackCount].ShotPreparation(move.GetPosition());//UŒ‚‚Ì€”õ
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
	}
	if (aimShotAttack[aimShotMoveCount-1].GetIsShotEnd())
	{
			//‰Šú‰»ˆ—	
		for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
		{
			aimShotAttack[attackCount].ShotReserve();
		}
		aimShotMoveCount = 1;
		attackMode = 1;
	}
	break;
	default:
		attackMode = 1;
	};
}

void Boss::Render(DirectX::SpriteBatch* SpriteBatch)
{	
	move.Render(SpriteBatch, 0);
	switch (attackMode)
	{
	case 1:
		//’e‚Ì•`‰æ
		for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++)
		{
			frameAttack.Render(SpriteBatch, bulletNumber);
		}
		//UŒ‚‰ñ”•`‰æ
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			//’e‚Ì•`‰æ
			for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
			{
				normalAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	case 2:
		//’e‚Ì•`‰æ
		inductionAttack.Render(SpriteBatch, 0);
		break;
	case 3:
		for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
		{
			//’e‚Ì•`‰æ
			for (int bulletNumber = 0; bulletNumber < AIMSHOT_BULLET_MAX; bulletNumber++)
			{
				aimShotAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	};
}
