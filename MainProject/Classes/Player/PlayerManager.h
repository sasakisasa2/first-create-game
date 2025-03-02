/*************************************************************
��File Name
�@  PlayerManager.h(�v���C���[�t�@�C���̒�)
��Function(�@�\)
�@  �v���C���[�̈ړ���T�C�Y�ȂǏڍׂȕύX���������ϐ���Z�߂�
    ����class
��Info(�ڍ׏��)�@
�@  �摜�̕`��Ȃǂ������ł���Ă�
��Other Use(���g�p)
�@  �v���C���[�̃T�C�Y�⑬�x�������Ă��������ꍇ�͂����ɂ���
    (�h���N���X�ł��Ăяo���͂ł���j
��Note(���l)
�@�@�v���C���[��position�̂�Move�������Ă���
�@  �p�����Ďg���Ă���(Move��Bullet�j
��Error(���)�@
�@  ���ɂȂ�
**************************************************************/
#pragma once
#include<vector>
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
class PlayerManager/*:public SpriteRender*/
{
private:

//********************�v���C���[�֌W**********************
	int   attribute ;    //�v���C���[����(1,Blue|2,Red|3,Black)
	int   spriteSize;    //100
	float moveSpeed ;    //300.0f
//********************************************************

//********************�v���C���[�֌W**********************
	Vector2 playerPosition  ;//�v���C���[�̈ʒu
	Vector2 attackSpriteSize;//�U���e�̃T�C�Y(5��3)
	int     attackWayMax    ;//WAY�� 
	float   attackInterval  ;//�U���Ԋu
	float   attackDamage    ;//�U���_���[�W
	float   attackSpeedX    ;//�U���e�̃X�s�[�h
	float   attackWayY      ;//�e���m�̊Ԋu
//********************************************************
protected:		

#pragma region �h��class��sprite��position,color
	DirectXTK::Sprite sprite;

	std::vector<SimpleMath::Vector2> position; 
	std::vector<float> angle;
	Color color;
#pragma endregion


#pragma region �A�N�Z�T
//***********************�v���C���[�A�N�Z�T*************************
#pragma region �����A�N�Z�T
	void SetAttribute(int getAttribute);

	int GetAttribute();
#pragma endregion
#pragma region �v���C���[�T�C�Y�A�N�Z�T
	void SetSpriteSize(int size);

	int GetSpriteSize();
#pragma endregion
#pragma region �v���C���[�X�s�[�h�A�N�Z�T
	void SetMoveSpeed(float speed);

	float GetMoveSpeed();
#pragma endregion
//******************************************************************

//****************************�U���֌W******************************
#pragma region playerPosition�A�N�Z�T
	void SetPosition(Vector2 plPosition);
	Vector2 GetPosition();
#pragma endregion
#pragma region �U���T�C�Y�A�N�Z�T
	void SetAttackSpriteSize(Vector2 setSize);

	Vector2 GetAttackSpriteSize();
#pragma endregion
#pragma region �U��WAY���A�N�Z�T
	void SetAttackWay(int attackWay);
	int  GetAttackWay();
#pragma endregion
#pragma region �U���Ԋu�A�N�Z�T
	void SetAttackInterval(float setInterval);

	float GetAttackInterval();
#pragma endregion
#pragma region �U���_���[�W�A�N�Z�T
	void SetAttackDamage(float setDamege);

	float GetAttackDamage();
#pragma endregion
#pragma region �U���X�s�[�h�A�N�Z�T
	void  SetAttackSpeed(float setAtttackSpped);

	float GetAttackSpeed();
#pragma endregion
#pragma region �e�̊Ԋu�A�N�Z�T
	void SetAttackWayY(float setAttackWayY);

	float GetAttackWayY();
#pragma endregion

//******************************************************************
#pragma endregion

public: 
	static constexpr int PLAYER_ATTACK_MAX = 100;//�e�̑S�̐�
	
	PlayerManager();

	/// <summary>
	/// �摜�Ăяo��
	/// </summary>
	/// <param name="newSprite">�摜�̏���������sprite</param>
	void Load  (DirectXTK::Sprite& newSprite);

	/// <summary>
	/// �摜�`��
	/// </summary>
	/// <param name="spriteBarch">SpriteBatch   </param>
	/// <param name="count">      ����`�悷�邩</param>
	void Render(DirectX::SpriteBatch* spriteBarch,int count);

	/// <summary>
	/// ���W����ʂ̊O�փI�[�o�[���Ă��邩�m�F���A
	/// �I�[�o�[���Ă����ꍇ��ʓ��ɖ߂�
	/// </summary>
	/// <param name="position">���肵�����ʒu���  </param>
	/// <returns>�����̈ʒu(�����Ȃ���΂��̂܂�)</returns>
	SimpleMath::Vector2 PositionScreenOver(SimpleMath::Vector2 position);
};