#include"BitManager.h"

BitManager::BitManager():
	position(BIT_ATTACK_MAX, Vector2(0, 0)),
	color   (BIT_ATTACK_MAX, Vector4(1.0f,1.0f,1.0f,1.0f)),
	angle   (BIT_ATTACK_MAX, 0.0f)
{
	SetRotationAngle(-90.0f);				  //��]�����ɉ����ĉ摜����]�����邽�߂̕ϐ��ݒ�
	SetSpriteSize(Vector2(30.0f, 30.0f));	  //�摜�T�C�Y
	SetDistance(100.0f);					  //���S����̋���
	SetOmega(0.5f);							  //��]���x
	SetAttackSpriteSize(Vector2(30.0f,30.0f));//�U���摜�̃T�C�Y
	SetAttackSpace(Vector2(5.0f,5.0f))  ;	  //x,�U���S�̂̋Ȃ���||y,�e���m�̊Ԋu
}

#pragma region �A�N�Z�T

#pragma region ���ʃA�N�Z�T

#pragma region ��]�p�x�ݒ�p�A�N�Z�T
void  BitManager::SetRotationAngle(float setAngle) {
	rotationAngle = setAngle;
}
float BitManager::GetRotationAngle() {
	return rotationAngle;
}
#pragma endregion

#pragma endregion

#pragma region BitMove�̃A�N�Z�T

#pragma region Bit�̃T�C�Y
void    BitManager::SetSpriteSize(Vector2 setSize)
{
	spriteSize = setSize;
}
Vector2 BitManager::GetSpriteSize()
{
	return spriteSize;
}
#pragma endregion

#pragma region ���S���W����̋���
void  BitManager::SetDistance(float setDistance)
{
	distance = setDistance;
}
float BitManager::GetDistance()
{
	return distance;
}
#pragma endregion

#pragma region ��]���x
void  BitManager::SetOmega(float setOmega)
{
	omega = setOmega;
}
float BitManager::GetOmega()
{
	return omega;
}
#pragma endregion

#pragma endregion

#pragma region BitAttack�̃A�N�Z�T

#pragma region �U���T�C�Y
void    BitManager::SetAttackSpriteSize(Vector2 setAttackSize)
{
	attackSpriteSize = setAttackSize;
}
Vector2 BitManager::GetAttackSpriteSize()
{
	return attackSpriteSize;
}
#pragma endregion

#pragma region �e���m�̊Ԋu
void    BitManager::SetAttackSpace(Vector2 setAttackSpace)
{
	attackSpace = setAttackSpace;
}
Vector2 BitManager::GetAttackSpace()
{
	return attackSpace;
}
#pragma endregion

#pragma endregion

#pragma endregion


Vector2 BitManager::SquareMovement(float theta,Vector2 centerPosition,float distance)
{
	//�w�肳�ꂽ�p�x���֐��p�ɒl��ݒ�
	double angle = theta/57.295791f;
	Vector2 differencePosition;
	//���W���Q�Ƃ��ʒu���ړ�
	differencePosition.x = centerPosition.x + distance * cos(angle);
	differencePosition.y = centerPosition.y + distance * sin(angle);
	return differencePosition;
}

void BitManager::Load(DirectXTK::Sprite& newSprite)
{
	sprite = newSprite;
}

void BitManager::Render(DirectX::SpriteBatch* spriteBatch, int count)
{
	for (int number = 0; number < count; number++) 
	{
		spriteBatch->Draw(
			sprite.handle, sprite.size, position[number], //�`��
			nullptr,									  //�摜�̂ǂ��ɕ\�����邩
			color[number],                                //�F�̊Ԉ�����(�J���[�}�X�N)
			angle[number] * Mathf::Deg2Rad,               //��]�̊p�x
			Vector2(sprite.size.x / 2, sprite.size.y / 2),//��
			1.0f										  //�摜�T�C�Y(Sprite�̃T�C�Y�ł͂Ȃ�)
		);
	}
}
