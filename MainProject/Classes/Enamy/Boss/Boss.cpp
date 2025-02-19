#include "Boss.h"

void Boss::Load(DirectXTK::Sprite& bossSprite, DirectXTK::Sprite& atttackSprite, DirectXTK::Sprite& hpSprite, DirectXTK::Sprite& hpBackGroundSprite)
{
	move           .Load(bossSprite);
//	normalAttack   .Load(atttackSprite);
	//inductionAttack.Load(atttackSprite);
	//frameAttack    .Load(atttackSprite);
	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].Load(atttackSprite);
	}
}

void Boss::Initialize()
{
	//move           .SpriteSizeSet(GetBossSpriteSize());
	//normalAttack   .SpriteSizeSet(GetNormalSpriteSize());
	//inductionAttack.SpriteSizeSet(GetInductionSpriteSize());
	//frameAttack    .SpriteSizeSet(GetFrameSpriteSize());
	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		aimShotAttack[attackCount].SpriteSizeSet(GetAimShotSpriteSize());
	}
	//frameAttack.PositionSet();
}

void Boss::Update(Vector2 playerPosition)
{
	//switch (attackMode)
	//{
	//case 1:
	//	frameAttack.Update();
	//	if (modeChangeCoolTime.TimeMeasurement(2))
	//	{
	//		modeChangeCoolTime.TimerReSet();
	//		attackMode++;
	//	}
	////	break;
	////case 2:	

	//	if (normalAttack.GetIsMoveStart())
	//	{
	//		if (normalAttack.GetAttackCount() % 2 == 0)
	//		{
	//			move.Update(-1.0f);
	//		}
	//		else
	//		{
	//			move.Update(1.0f); 
	//		}
	//	}
	//	if (move.GetIsMoveEnd())
	//	{	
	//		if (normalAttack.GetAttackCount() >= normalAttack.NORMAL_SHOT_COUNT)
	//		{
	//			if (modeChangeCoolTime.TimeMeasurement(2))
	//			{
	//				modeChangeCoolTime.TimerReSet();
	//				inductionAttack.InductionStart(move.GetPosition());
	//				attackMode++;
	//			}
	//		}
	//		else
	//		{
	//			normalAttack.SetIsMoveStart(false);
	//			move.SetIsMoveEnd(false);
	//		}
	//	}
	//	normalAttack.ShotMove();

	//	break;
	//case 3:
	//if(test)
	//{
	//	normalAttack.ShotPreparation(move.GetPosition());
	//	inductionAttack.InductionStart(move.GetPosition());
	//	test = false;
	//}
	//	inductionAttack.Update(playerPosition);
	//	if (modeChangeCoolTime.TimeMeasurement(2))
	//	{
	//		modeChangeCoolTime.TimerReSet();
	//		attackMode++;
	//	}
	//	break;
	//case 4:
	for (int attackCount = 0; attackCount < aimShotMoveCount; attackCount++)
	{
		if (aimShotTimer[attackCount].TimeMeasurement(3))
		{
			aimShotAttack[attackCount].Update(move.GetPosition(), playerPosition);
			if (attackCount == aimShotMoveCount-1) { aimShotMoveCount++; }
		}
	}
		if (modeChangeCoolTime.TimeMeasurement(2))
		{
			modeChangeCoolTime.TimerReSet();
			attackMode = 1;
		}
	//	break;
	//default:
		attackMode = 1;
	};
//}

void Boss::Render(DirectX::SpriteBatch* SpriteBatch)
{	
	move.Render(SpriteBatch, 0);

	////switch (attackMode)
	////{
	////case 1:
	//	for (int bulletNumber = 0; bulletNumber < frameAttack.FRAME_BULLET_MAX; bulletNumber++)
	//	{
	//		frameAttack.Render(SpriteBatch, bulletNumber);
	//	}
	////	break;
	////case 2:
	//	for (int bulletNumber = 0; bulletNumber < normalAttack.NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	//	{
	//		normalAttack.Render(SpriteBatch, bulletNumber);
	//	}
	////	break;
	////case 3:
		//inductionAttack.Render(SpriteBatch,0);
	////	break;
	////case 4:
	for (int attackCount = 0; attackCount < 5; attackCount++)
	{
		for (int bulletNumber = 0; bulletNumber < aimShotAttack[0].AIMSHOT_ONEATTACK_BULLET_MAX; bulletNumber++)
		{
			aimShotAttack[attackCount].Render(SpriteBatch, bulletNumber);
		}
	}
	////	break;
	////default:
	//	attackMode = 1;
	//	//break;
	};
//
//}
