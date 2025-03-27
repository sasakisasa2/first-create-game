/*************************************************************
��File Name
�@  BitManager.h
��Function(�@�\)
�@  Bit�̍U���ƈړ��̕ϐ��̏ڍאݒ�
��Info(�ڍ׏��)�@
�@  Bit�Ɋ֘A���鋤�ʂ̕ϐ���ڍׂȐݒ���ł���ϐ�����Z�߂ĊǗ�����悤�ɍ쐬
��Other Use(���g�p)
	�ق����ϐ���getter�֐��Ȃǂ��Ăяo���Ă�������
��Note(���l)
�@�@�r�b�g�̍��W�⓮�����Ă�ϐ��͓������Ă���N���X����Ăяo���Ă�������
��Error(���)�@
�@  ���̂Ƃ�����Ȃ�
**************************************************************/

#pragma once
#include<vector>
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
using namespace CF;

class BitManager 
{
private:

#pragma region ����
	float rotationAngle;
#pragma endregion
#pragma region BitMove
	Vector2 spriteSize;//�r�b�g�̑傫��
	float   distance;  //�v���C���[�ƃr�b�g�̋���
	float   omega;     //�r�b�g�̉�]��
#pragma endregion
#pragma region BitAttack
	Vector2 attackSpriteSize;//�r�b�g�̍U���̒e�̑傫��
	Vector2 attackSpace;     //�e���m�̋���
#pragma endregion

protected:

#pragma region �h���N���X�̉摜�`��p
	std::vector<Vector2> position;
	DirectXTK::Sprite    sprite  ;
	std::vector<Color>   color   ;
	std::vector<float>   angle   ;
#pragma endregion

	BitManager();

#pragma region �A�N�Z�T
#pragma region ���ʃA�N�Z�T
#pragma region �p�x�ݒ�p�A�N�Z�T
	void  SetRotationAngle(float setAngle);
	float GetRotationAngle();
#pragma endregion
#pragma endregion
#pragma region BitMove�̃A�N�Z�T
#pragma region �摜�T�C�Y
	void    SetSpriteSize(Vector2 setSize);
	Vector2 GetSpriteSize();
#pragma endregion
#pragma region ���S���W����Bit�̋���
	void    SetDistance(float setDistance);
	float   GetDistance();
#pragma endregion
#pragma region ��]���x
	void    SetOmega(float omega);
	float   GetOmega();
#pragma endregion
#pragma endregion
#pragma region BitAttack�̃A�N�Z�T
#pragma region �r�b�g�̍U���T�C�Y
	void    SetAttackSpriteSize(Vector2 setAttackSize);
	Vector2 GetAttackSpriteSize();
#pragma endregion
#pragma region �r�b�g�̍U���e�̊Ԋu
	void    SetAttackSpace(Vector2 setAttackSpace);
	Vector2 GetAttackSpace();
#pragma endregion
#pragma endregion
#pragma endregion

public:

	static constexpr int BIT_ATTACK_MAX = 100;//�U���̍ő吔

	void Load(DirectXTK::Sprite& newSprite);
	/// <summary>
	/// �摜�`��
	/// </summary>
	/// <param name="spriteBarch">SpriteBatch   </param>
	/// <param name="count">      ����`�悷�邩</param>
	void Render(DirectX::SpriteBatch* spriteBarch, int count);
};