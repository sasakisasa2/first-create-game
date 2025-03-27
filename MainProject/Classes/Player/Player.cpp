#include"Player.h"

using namespace DirectXTK;

using namespace SimpleMath;

void Player::LoadAssets(Sprite& player, Sprite& Attack, Sprite& collision) 
{
	move.         Load(player);
	attack.       Load(Attack);
	colliderSprite.Load(collision);
}

void Player::Initialize()
{
	playerPosition = Vector2::Zero;
	move.         Initialize();
	attack.       Initialize();
	colliderSprite.Initialize();
}

void Player::Update() 
{
	move.		  Update();
	playerPosition = move.GetPlayerPosition();

    attack.		  Update(playerPosition);
	colliderSprite.Update(playerPosition);

	const int   THREE = 3;
	const float ONE   = 1;

	playerCollisionInfo.SetSquareCorner(playerPosition, 
		Vector2(GetSpriteSize() / THREE, GetSpriteSize() / THREE),
		move.GetVector(),
		move.GetAngle(), 
		GetMoveSpeed());

	for (int bulletNumber = 0; bulletNumber < PLAYER_ATTACK_MAX; bulletNumber++)
	{
		if (attack.GetIsShot(bulletNumber))
		{
			attackCollisionInfo[bulletNumber].SetSquareCorner(playerPosition,
				GetAttackSpriteSize(),
				attack.GetShotVector(bulletNumber),
				attack.GetBulletAngle(bulletNumber),
				ONE);
		}
	}
}

void Player::Render(DirectX::SpriteBatch* SpriteBatch) 
{
	move.         Render(SpriteBatch,1);
	attack.       Render(SpriteBatch,attack.PLAYER_ATTACK_MAX);
	colliderSprite.Render(SpriteBatch,1);
}
