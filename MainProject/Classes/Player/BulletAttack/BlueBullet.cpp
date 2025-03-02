#include"BlueBullet.h"
using namespace CF;
using namespace SimpleMath;

Bullet::Bullet()
{
	const float MINUS = -1;
	for (int bulletCount = 0; bulletCount < PLAYER_ATTACK_MAX; bulletCount += GetAttackWay())
	{
		for (int wayCount = 0; wayCount < GetAttackWay(); wayCount++)
		{
			if (wayCount <= wayHalf)
			{
				//^‚ñ’†‚Æã•ûŒü‚ÌWAY
				shotVector[wayCount + bulletCount] = Vector2(GetAttackSpeed(), GetAttackWayY() * wayCount);
			}
			else
			{
				//‰º•ûŒü‚ÌWAY
				shotVector[wayCount + bulletCount] = Vector2(GetAttackSpeed(), (GetAttackWayY() * (wayCount - wayHalf))* MINUS);
			}
		}
	}
}

void  Bullet::Initialize()
{
	sprite.size.x = GetAttackSpriteSize().x;
	sprite.size.y = GetAttackSpriteSize().y;

	for (int i = 0; i < PLAYER_ATTACK_MAX; i++)
	{
		attackFlag[i] = false;
	}
}

#pragma region ƒvƒŒƒCƒ„[ˆÈŠO‚ÅŽg‚¤Initialize
void Bullet::OtherInitialize(Vector2 size, float interval, float speed, float wayY)
{
	for (int i = 0; i < PLAYER_ATTACK_MAX; i++)
	{
		attackFlag[i] = false;
	}
//*****************************************************
//’lÝ’è
	SetAttackSpriteSize(size)    ;
	SetAttackInterval  (interval);
	SetAttackSpeed     (speed)   ;
	SetAttackWayY      (wayY)    ;
//*****************************************************
	sprite.size.x = GetAttackSpriteSize().x;
	sprite.size.y = GetAttackSpriteSize().y;
}

#pragma endregion


void  Bullet::Update(SimpleMath::Vector2 shotPosition) 
{
//*****************************************************
//’lÝ’è
	SetPosition(shotPosition);
	wayHalf = GetAttackWay() / 2;
//*****************************************************
	if (InputSystem.Keyboard.isPressed.Space)
	{
	//’e‚Ì€”õ
	Bullet::Preparation();
	}

	//’e‚ðŒ‚‚Â
	Bullet::Shot();

	//’e‚ÌUŒ‚Š®—¹Žž‚Ì‰Šú‰»ˆ—
	Bullet::BulletReturn();
}

#pragma region ƒvƒŒƒCƒ„[ˆÈŠO‚ÅŽg‚¤Û‚ÌUpdate
void Bullet::OtherUpdate(Vector2 otherPosition, int otherAttribute, int otherWay)
{
//***************************************************
//’lÝ’è
	SetPosition (otherPosition) ;
	SetAttribute(otherAttribute);
	SetAttackWay(otherWay)      ;
	wayHalf = GetAttackWay() / 2;
//***************************************************

	//’e‚Ì€”õ
	Bullet::Preparation();

	//’e‚ðŒ‚‚Â
	Bullet::Shot();

	//’e‚ÌUŒ‚Š®—¹Žž‚Ì‰Šú‰»ˆ—
	Bullet::BulletReturn();
}
#pragma endregion

//’e‚ðŒ‚‚Â€”õ
void Bullet::Preparation()
{
	const bool isAttack = timerCount.TimeMeasurement(GetAttackInterval());//0.05
	if (!isAttack)
		return;

	//’e‚Ì”ŒJ‚è•Ô‚·
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay())
	{
		//Žw’èŽžŠÔŒo‚Ä‚ÎŒ‚‚¿Žn‚ß‚é
		if (!attackFlag[i])
		{
			//Ž©•ª‚Ì‘®«‚Ì’e‚Ì€”õ
			if (GetAttribute() == 1) { Bullet::BluePreparation(i); }
			if (GetAttribute() == 2) { Bullet::RedPreparation (i); }
			timerCount.TimerReSet();
			break;
		}
	}
}

#pragma region Žw’è‘®«‚Ì’e‚ð€”õ
void Bullet::BluePreparation(int bulletNumber) 
{
	for (int j = 0; j < GetAttackWay(); j++)
	{
		position  [bulletNumber + j] = GetPosition();
		attackFlag[bulletNumber + j] = true;
	}
}

void Bullet::RedPreparation (int bulletNumber)
{
	//way‚Ì”¼•ª‚ðã•ûŒüA”¼•ª‚ð‰º•ûŒü’u‚­
	for (int j = 0; j < GetAttackWay(); j++) 
	{
		if (j <= wayHalf)
		{
			//^‚ñ’†‚Æã•ûŒü‚ÌWAY
			position[bulletNumber + j] =
				Vector2(GetPosition().x,
				        GetPosition().y + (j * GetAttackWayY())
	                    );
		}
		else 
		{
			//‰º•ûŒü‚ÌWAY
			position[bulletNumber + j] =
			    Vector2(GetPosition().x,
				        GetPosition().y - (GetAttackWayY() * (j - wayHalf))
					    );
		}
		attackFlag[j + bulletNumber] = true;
	}
}

#pragma endregion

void Bullet::Shot()
{
	//UŒ‚€”õ‚ªŠ®—¹‚µ‚Ä‚¢‚é’e‚ð‘Å‚¿o‚·
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay()) 
	{
		for (int j = 0; j < GetAttackWay(); j++)
		{
			//Ž©g‚Ì‘®«‚Ì’e‚ð‘Å‚¿o‚·
			if (attackFlag[j + i])
			{
				if (GetAttribute() == 1){ Bullet::ShotBlue(j, i);}
				if (GetAttribute() == 2){ Bullet::ShotRed (j, i);}
			}
		}
	}
}

void Bullet::ShotBlue(int wayCount,int bulletCount)
{
	if (wayCount <= wayHalf) 
	{
		//^‚ñ’†‚Æã•ûŒü‚ÌWAY
		position[wayCount + bulletCount] += shotVector[wayCount + bulletCount] * DXTK->Time.deltaTime;
	}
	else 
	{
		//‰º•ûŒü‚ÌWAY
		position[wayCount + bulletCount] += shotVector[wayCount + bulletCount] * DXTK->Time.deltaTime;	}
}

void Bullet::ShotRed (int wayCount,int bulletCount)
{
	position[wayCount+ bulletCount].x += GetAttackSpeed() * DXTK->Time.deltaTime;
}

void Bullet::BulletReturn()
{
	for (int i = 0; i < PLAYER_ATTACK_MAX; i++)
	{
		if (position[i].x >= DXTK->Screen.Width+(sprite.size.x/2))
		{
			attackFlag[i] = false;
		}
		if (!attackFlag[i]) 
		{
			position[i] = Vector2(-100, -100);
		}
	}
}

