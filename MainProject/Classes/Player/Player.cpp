#include"Player.h"

using namespace DirectXTK;

using namespace SimpleMath;

void Player::LoadAssets(Sprite& player, Sprite& Attack, Sprite& collision) 
{
	move.         Load(player);
	attack.       Load(Attack);
	coliderSprite.Load(collision);
}

void Player::Initialize()
{
	playerPosition = Vector2::Zero;
	move.         Initialize();
	attack.       Initialize();
	coliderSprite.Initialize();
}

void Player::Update() 
{
	move.		  Update();
	playerPosition = move.ReturnPlayerPosition();

    attack.		  Update(playerPosition);
	coliderSprite.Update(playerPosition);
}

void Player::Render(DirectX::SpriteBatch* SpriteBatch) 
{
	move.         Render(SpriteBatch,1);
	attack.       Render(SpriteBatch,attack.PLAYER_ATTACK_MAX);
	coliderSprite.Render(SpriteBatch,1);
}
