
#include"PlayerManager.h"

#include"..\MainProject\Scenes\DontDestroyOnLoad.h"
	//�l�̕ύX�͂�����
PlayerManager::PlayerManager() : position(PLAYER_ATTACK_MAX+3,SimpleMath::Vector2(300,300)),angle(PLAYER_ATTACK_MAX,0.0f)
{
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	SetAttribute       (DontDestroy->attackAttribute);
	SetSpriteSize      (100);
	SetMoveSpeed       (500.0f);
	SetPosition        (Vector2(300.0f, 300.0f));
	SetAttackSpriteSize(Vector2(50, 30));
	SetAttackWay       (3);
	SetAttackInterval  (0.1);
	SetAttackDamage    (5);
	SetAttackSpeed     (1000.0f);
	SetAttackWayY       (50.0f);
}

#pragma region �A�N�Z�T
//***************************�v���C���[�A�N�Z�T*****************************
#pragma region �����A�N�Z�T
void PlayerManager::SetAttribute(int getAttribute)
{
	attribute = getAttribute;
}

int PlayerManager::GetAttribute()
{
	return attribute;
}

#pragma endregion
#pragma region �v���C���[�T�C�Y�A�N�Z�T

void PlayerManager::SetSpriteSize(int size)
{
	spriteSize = size;
}

int PlayerManager::GetSpriteSize()
{
	return spriteSize;
}

#pragma endregion
#pragma region �v���C���[�X�s�[�h�A�N�Z�T
void PlayerManager::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
}

float PlayerManager::GetMoveSpeed()
{
	return moveSpeed;
}
#pragma endregion
//**************************************************************************

//****************************�A�^�b�N�A�N�Z�T******************************
#pragma region playerPosition�A�N�Z�T
void PlayerManager::SetPosition(Vector2 plPosition) {
	playerPosition = plPosition;
}
Vector2 PlayerManager::GetPosition() {
	return playerPosition;
}
#pragma endregion
#pragma region �U���摜�T�C�Y�A�N�Z�T
void PlayerManager::SetAttackSpriteSize(Vector2 setSize)
{
	attackSpriteSize = setSize;
}

Vector2 PlayerManager::GetAttackSpriteSize()
{
	return attackSpriteSize;
}
#pragma endregion
#pragma region �U��WAY�A�N�Z�T
void PlayerManager::SetAttackWay(int attackWay) {
	attackWayMax = attackWay;
}
int  PlayerManager::GetAttackWay() {
	return attackWayMax;
}
#pragma endregion
#pragma region �U���C���^�[�o���A�N�Z�T
void PlayerManager::SetAttackInterval(float setInterval)
{
	attackInterval = setInterval;
}

float PlayerManager::GetAttackInterval()
{
	return attackInterval;
}
#pragma endregion
#pragma region �U���_���[�W�A�N�Z�T
void PlayerManager::SetAttackDamage(float setDamege)
{
	attackDamage = setDamege;
}

float PlayerManager::GetAttackDamage()
{
	return attackDamage;
}
#pragma endregion
#pragma region �A�^�b�N�X�s�[�h�A�N�Z�T
void PlayerManager::SetAttackSpeed(float setAttackSpeed)
{
	attackSpeedX = setAttackSpeed;
}

float PlayerManager::GetAttackSpeed()
{
	return attackSpeedX;
}
#pragma endregion
#pragma region �U��WayY�A�N�Z�T
void PlayerManager::SetAttackWayY(float setAttackWayY)
{
	attackWayY = setAttackWayY;
}

float PlayerManager::GetAttackWayY()
{
	return attackWayY;
}
#pragma endregion
//**************************************************************************
#pragma endregion

#pragma region �摜�ǂݎ��
void PlayerManager::Load(DirectXTK::Sprite& newSprite) 
{
	sprite = newSprite;
}
#pragma endregion

#pragma region �摜�`��
void PlayerManager::Render(DirectX::SpriteBatch* spriteBatch, int count) 
{
	for (int number = 0; number < count; number++) 
	{
		spriteBatch->Draw(
			sprite.handle, sprite.size, position[number],
			nullptr,//�摜�̂ǂ��ɕ\�����邩
			color,//�F�̊Ԉ�����(�J���[�}�X�N)
			angle[number] * Mathf::Deg2Rad, SimpleMath::Vector2(sprite.size.x / 2, sprite.size.y / 2),//��]�̊p�x�Ǝ�
			1.0f
		);
	}
}
#pragma endregion

SimpleMath::Vector2 PlayerManager::PositionScreenOver(SimpleMath::Vector2 position)
{
		if (position.x >= DXTK->Screen.Width - sprite.size.x / 2) 
		{
			position.x = DXTK->Screen.Width - sprite.size.x / 2;
		}
		if (position.x <= 0 + sprite.size.x / 2) 
		{
			position.x = 0 + sprite.size.x / 2;
		}
		if (position.y >= DXTK->Screen.Height - sprite.size.y / 2) 
		{
			position.y = DXTK->Screen.Height - sprite.size.y / 2;
		}
		if (position.y <= 0 + sprite.size.y / 2)
		{
			position.y = 0 + sprite.size.y / 2;
		}
	return position;
}


