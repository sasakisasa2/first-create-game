#include "Boss.h"

void Boss::Load(DirectXTK::Sprite& bossSprite, DirectXTK::Sprite& atttackSprite, DirectXTK::Sprite& hpSprite, DirectXTK::Sprite& hpBackGroundSprite)
{
	move           .Load(bossSprite);
	normalAttack   .Load(atttackSprite);
	inductionAttack.Load(atttackSprite);
	frameAttack    .Load(atttackSprite);
	aimShotAttack  .Load(atttackSprite);
}

void Boss::Initialize()
{
	move           .SpriteSizeSet(GetBossSpriteSize());
	normalAttack   .SpriteSizeSet(GetNormalSpriteSize());
	inductionAttack.SpriteSizeSet(GetInductionSpriteSize());
	frameAttack    .SpriteSizeSet(GetFrameSpriteSize());
	aimShotAttack  .SpriteSizeSet(GetAimShotSpriteSize());
	frameAttack.PositionSet();
}

void Boss::Update(Vector2 playerPosition)
{
	//switch (attackMode)
	//{
	//case 1:
		frameAttack.Update();
		if (modeChangeCoolTime.TimeMeasurement(2))
		{
			modeChangeCoolTime.TimerReSet();
			attackMode++;
		}
	//	break;
	//case 2:	

		if (normalAttack.GetIsMoveStart())
		{
			if (normalAttack.GetAttackCount() % 2 == 0)
			{
				move.Update(-1.0f);
			}
			else
			{
				move.Update(1.0f); 
			}
		}
		if (move.GetIsMoveEnd())
		{	
			if (normalAttack.GetAttackCount() >= normalAttack.NORMAL_SHOT_COUNT)
			{
				if (modeChangeCoolTime.TimeMeasurement(2))
				{
					modeChangeCoolTime.TimerReSet();
					inductionAttack.InductionStart(move.GetPosition());
					attackMode++;
				}
			}
			else
			{
				normalAttack.SetIsMoveStart(false);
				normalAttack.ShotPreparation(move.GetPosition());
				move.SetIsMoveEnd(false);
			}
		}
		normalAttack.ShotMove();

	//	break;
	//case 3:
		//inductionAttack.Update(playerPosition);
		//if (modeChangeCoolTime.TimeMeasurement(2))
		//{
		//	modeChangeCoolTime.TimerReSet();
		//	attackMode++;
		//}
	//	break;
	//case 4:
		//aimShotAttack.Update(move.GetPosition(), playerPosition);
		//if (modeChangeCoolTime.TimeMeasurement(2))
		//{
		//	modeChangeCoolTime.TimerReSet();
		//	attackMode = 1;
		//}
	//	break;
	//default:
		attackMode = 1;
	};
//}

void Boss::Render(DirectX::SpriteBatch* SpriteBatch)
{	
	move.Render(SpriteBatch, 0);

	//switch (attackMode)
	//{
	//case 1:
		for (int bulletNumber = 0; bulletNumber < frameAttack.FRAME_BULLET_MAX; bulletNumber++)
		{
			frameAttack.Render(SpriteBatch, bulletNumber);
		}
	//	break;
	//case 2:
		for (int bulletNumber = 0; bulletNumber < normalAttack.NORMAL_SHOT_BULLET_MAX; bulletNumber++)
		{
			normalAttack.Render(SpriteBatch, bulletNumber);
		}
	//	break;
	//case 3:
		inductionAttack.Render(SpriteBatch,0);
	//	break;
	//case 4:
		for (int bulletNumber = 0; bulletNumber < aimShotAttack.AIMSHOT_BULLET_MAX; bulletNumber++)
		{
			aimShotAttack.Render(SpriteBatch, bulletNumber);
		}
	//	break;
	//default:
		attackMode = 1;
		//break;
	};
//
//}
