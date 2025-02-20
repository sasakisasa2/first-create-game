#include "Boss.h"

void Boss::Load(DirectXTK::Sprite& bossSprite, DirectXTK::Sprite& atttackSprite, DirectXTK::Sprite& hpSprite, DirectXTK::Sprite& hpBackGroundSprite)
{
	move.Load(bossSprite);
	
	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		normalAttack[attackCount].Load(atttackSprite);
	}

	inductionAttack.Load(atttackSprite);

	frameAttack.Load(atttackSprite);

	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].Load(atttackSprite);
	}
}

void Boss::Initialize()
{
	move.SpriteSizeSet(GetBossSpriteSize());

	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		normalAttack[attackCount].SpriteSizeSet(GetNormalSpriteSize());
	}

	inductionAttack.SpriteSizeSet(GetInductionSpriteSize());

	frameAttack.SpriteSizeSet(GetFrameSpriteSize());

	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].SpriteSizeSet(GetAimShotSpriteSize());
	}

	frameAttack.PositionSet();
}

void Boss::Update(Vector2 playerPosition)
{
	switch (attackMode)
	{
	case 1:
		frameAttack.Update();
		if (modeChangeCoolTime.TimeMeasurement(2))
		{
			modeChangeCoolTime.TimerReSet();
			normalAttack[0].ShotPreparation(move.GetPosition());
			isNormalAttackShot[0] = true;
			attackMode++;
		}
		break;
	case 2:	
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			if (isNormalAttackShot[attackCount])
			{
				float vector = 1.0f;
				//ã‚©‚ç‰º‚Ö‚ÌUŒ‚AˆÚ“®
				if (attackCount % 2 == 1) { vector = -1.0f; }

				normalAttack[attackCount].ShotMove(vector);
			}
		}

		//ÅIUŒ‚‚ªI‚í‚é‚Ü‚Åì“®
		if (normalAttackCount != NORMAL_SHOT_COUNT)
		{
			//ã‚©‚ç‰º‚Ö‚ÌUŒ‚AˆÚ“®
			if (normalAttackCount % 2 == 0)
			{
				move.Update(1.0f);
			}
			//‰º‚©‚çã‚Ö‚ÌUŒ‚AˆÚ“®
			else
			{
				move.Update(-1.0f);
			}
			if (move.GetIsMoveEnd())
			{
				//UŒ‚‰ñ”‚ÌƒJƒEƒ“ƒg
				normalAttackCount++;
				move.SetIsMoveEnd(false);
				//ŽŸ‚ÌUŒ‚‚Ì€”õ(ÅIUŒ‚‚ÌÛ‚Í”­“®‚µ‚È‚¢‚æ‚¤‚É)
				if (normalAttackCount != NORMAL_SHOT_COUNT)
				{
					isNormalAttackShot[normalAttackCount] = true;
					normalAttack[normalAttackCount].ShotPreparation(move.GetPosition());
				}
			}
		}
		else
		{
			if (modeChangeCoolTime.TimeMeasurement(5))
			{
				normalAttackCount = 0;
				modeChangeCoolTime.TimerReSet();
				inductionAttack.InductionStart(move.GetPosition());
				attackMode++;
			}
		}
		break;
	case 3:
		inductionAttack.Update(playerPosition);
		if (inductionAttack.GetVolume() == 1)
		{
			if (modeChangeCoolTime.TimeMeasurement(2))
			{
				modeChangeCoolTime.TimerReSet();
				attackMode++;
			}
		}
		break;
	case 4:
	for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
	{
		if (aimShotTimer[attackCount].TimeMeasurement(3))
		{
			aimShotAttack[attackCount].Update(move.GetPosition(), playerPosition);
			if (attackCount == aimShotMoveCount-1) { aimShotMoveCount++; }
		}
	}
	//‚Ì‚¿C³(”­“®‚µ‚Ä‚¢‚È‚¢j
	if (aimShotAttack[aimShotMoveCount-1].GetIsPlaceMove())
	{
		if (modeChangeCoolTime.TimeMeasurement(2))
		{
			for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
			{
				aimShotAttack[attackCount].ShotReserve();
			}
			modeChangeCoolTime.TimerReSet();
			attackMode = 1;
		}
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
		for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++)
		{
			frameAttack.Render(SpriteBatch, bulletNumber);
		}
		break;
	case 2:
		for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
		{
			for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
			{
				normalAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	case 3:
		inductionAttack.Render(SpriteBatch, 0);
		break;
	case 4:
		for (int attackCount = 0; attackCount < 5; attackCount++)
		{
			for (int bulletNumber = 0; bulletNumber < AIMSHOT_BULLET_MAX; bulletNumber++)
			{
				aimShotAttack[attackCount].Render(SpriteBatch, bulletNumber);
			}
		}
		break;
	};
}
