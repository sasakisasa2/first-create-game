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
				//�^�񒆂Ə������WAY
				shotVector[wayCount + bulletCount] = Vector2(GetAttackSpeed(), GetAttackWayY() * wayCount);
			}
			else
			{
				//��������WAY
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

#pragma region �v���C���[�ȊO�Ŏg��Initialize
void Bullet::OtherInitialize(Vector2 size, float interval, float speed, float wayY)
{
	for (int i = 0; i < PLAYER_ATTACK_MAX; i++)
	{
		attackFlag[i] = false;
	}
//*****************************************************
//�l�ݒ�
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
//�l�ݒ�
	SetPosition(shotPosition);
	wayHalf = GetAttackWay() / 2;
//*****************************************************
	if (InputSystem.Keyboard.isPressed.Space)
	{
	//�e�̏���
	Bullet::Preparation();
	}

	//�e������
	Bullet::Shot();

	//�e�̍U���������̏���������
	Bullet::BulletReturn();
}

#pragma region �v���C���[�ȊO�Ŏg���ۂ�Update
void Bullet::OtherUpdate(Vector2 otherPosition, int otherAttribute, int otherWay)
{
//***************************************************
//�l�ݒ�
	SetPosition (otherPosition) ;
	SetAttribute(otherAttribute);
	SetAttackWay(otherWay)      ;
	wayHalf = GetAttackWay() / 2;
//***************************************************

	//�e�̏���
	Bullet::Preparation();

	//�e������
	Bullet::Shot();

	//�e�̍U���������̏���������
	Bullet::BulletReturn();
}
#pragma endregion

//�e��������
void Bullet::Preparation()
{
	const bool isAttack = timerCount.TimeMeasurement(GetAttackInterval());//0.05
	if (!isAttack)
		return;

	//�e�̐��J��Ԃ�
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay())
	{
		//�w�莞�Ԍo�ĂΌ����n�߂�
		if (!attackFlag[i])
		{
			//�����̑����̒e�̏���
			if (GetAttribute() == 1) { Bullet::BluePreparation(i); }
			if (GetAttribute() == 2) { Bullet::RedPreparation (i); }
			timerCount.TimerReSet();
			break;
		}
	}
}

#pragma region �w�葮���̒e������
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
	//way�̔�����������A�������������u��
	for (int j = 0; j < GetAttackWay(); j++) 
	{
		if (j <= wayHalf)
		{
			//�^�񒆂Ə������WAY
			position[bulletNumber + j] =
				Vector2(GetPosition().x,
				        GetPosition().y + (j * GetAttackWayY())
	                    );
		}
		else 
		{
			//��������WAY
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
	//�U���������������Ă���e��ł��o��
	for (int i = 0; i < PLAYER_ATTACK_MAX; i += GetAttackWay()) 
	{
		for (int j = 0; j < GetAttackWay(); j++)
		{
			//���g�̑����̒e��ł��o��
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
		//�^�񒆂Ə������WAY
		position[wayCount + bulletCount] += shotVector[wayCount + bulletCount] * DXTK->Time.deltaTime;
	}
	else 
	{
		//��������WAY
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

