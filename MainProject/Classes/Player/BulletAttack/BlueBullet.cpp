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
				//真ん中と上方向のWAY
				shotVector[wayCount + bulletCount] = Vector2(GetAttackSpeed(), GetAttackWayY() * wayCount);
			}
			else
			{
				//下方向のWAY
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
		isShot[i] = false;
	}
}

#pragma region プレイヤー以外で使うInitialize
void Bullet::OtherInitialize(Vector2 size, float interval, float speed, float wayY)
{
	for (int i = 0; i < PLAYER_ATTACK_MAX; i++)
	{
		isShot[i] = false;
	}
//*****************************************************
//値設定
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
//値設定
	SetPosition(shotPosition);
	wayHalf = GetAttackWay() / 2;
//*****************************************************
	if (InputSystem.Keyboard.isPressed.Space)
	{
	//弾の準備
	Bullet::Preparation();
	}

	//弾を撃つ
	Bullet::Shot();

	//弾の攻撃完了時の初期化処理
	Bullet::BulletReturn();
}

#pragma region プレイヤー以外で使う際のUpdate
void Bullet::OtherUpdate(Vector2 otherPosition, int otherAttribute, int otherWay)
{
//***************************************************
//値設定
	SetPosition (otherPosition) ;
	SetAttribute(otherAttribute);
	SetAttackWay(otherWay)      ;
	wayHalf = GetAttackWay() / 2;
//***************************************************

	//弾の準備
	Bullet::Preparation();

	//弾を撃つ
	Bullet::Shot();

	//弾の攻撃完了時の初期化処理
	Bullet::BulletReturn();
}
#pragma endregion

//弾を撃つ準備
void Bullet::Preparation()
{
	const bool isAttack = timerCount.TimeMeasurement(GetAttackInterval());//0.05
	if (!isAttack)
		return;

	//弾の数繰り返す
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay())
	{
		//指定時間経てば撃ち始める
		if (!isShot[i])
		{
			//自分の属性の弾の準備
			if (GetAttribute() == 1) { Bullet::BluePreparation(i); }
			if (GetAttribute() == 2) { Bullet::RedPreparation (i); }
			timerCount.TimerReSet();
			break;
		}
	}
}

#pragma region 指定属性の弾を準備
void Bullet::BluePreparation(int bulletNumber) 
{
	for (int j = 0; j < GetAttackWay(); j++)
	{
		position  [bulletNumber + j] = GetPosition();
		isShot[bulletNumber + j] = true;
	}
}

void Bullet::RedPreparation (int bulletNumber)
{
	//wayの半分を上方向、半分を下方向置く
	for (int j = 0; j < GetAttackWay(); j++) 
	{
		if (j <= wayHalf)
		{
			//真ん中と上方向のWAY
			position[bulletNumber + j] =
				Vector2(GetPosition().x,
				        GetPosition().y + (j * GetAttackWayY())
	                    );
		}
		else 
		{
			//下方向のWAY
			position[bulletNumber + j] =
			    Vector2(GetPosition().x,
				        GetPosition().y - (GetAttackWayY() * (j - wayHalf))
					    );
		}
		isShot[j + bulletNumber] = true;
	}
}

#pragma endregion

void Bullet::Shot()
{
	//攻撃準備が完了している弾を打ち出す
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay()) 
	{
		for (int j = 0; j < GetAttackWay(); j++)
		{
			//自身の属性の弾を打ち出す
			if (isShot[j + i])
			{
				if (GetAttribute() == 1){ Bullet::ShotBlue(j, i);}
				if (GetAttribute() == 2){ Bullet::ShotRed (j, i);}
			}
		}
	}
}

void Bullet::ShotBlue(int wayCount,int bulletCount)
{
		//真ん中と上方向のWAY
		position[wayCount + bulletCount] += shotVector[wayCount + bulletCount] * DXTK->Time.deltaTime;
		//角度の設定
		angle[wayCount + bulletCount] = CF::ChangeVectorToAngle(shotVector[wayCount + bulletCount], angle[wayCount + bulletCount]);
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
			isShot[i] = false;
		}
		if (!isShot[i]) 
		{
			position[i] = Vector2(-100, -100);
		}
	}
}

