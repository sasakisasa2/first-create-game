/*************************************************************
��File Name
�@  Bullet.h <- �㏑������(�v���C���[�t�@�C����BulletAttack�t�@�C���̒�)
��Function(�@�\)
�@  �e�̔���
��Info(�ڍ׏��)�@
�@  �v���C���[�Ŏg�p��z�肵�쐬
��Other Use(���g�p)
�@  OtherUpdate��OtherInitialize���Ăяo���Ŏg�p��
��Note(���l)
�@�@�v���C���[�Ŏg������PlayerManager�ŏڍאݒ�\
��Error(���)�@
�@  ���̂Ƃ�����Ȃ�
**************************************************************/
#pragma once
#include"../MainProject/Classes/Player/PlayerManager.h"


using namespace DirectX;

using namespace SimpleMath;

class Bullet : public PlayerManager
{
private:
	CF::Timer timerCount;

	Vector2 shotVector[PLAYER_ATTACK_MAX];//������
	int     wayHalf;					  //WAY���̔���
	bool    attackFlag[PLAYER_ATTACK_MAX];//�U�����邩�ǂ���

#pragma region �U�������p�֐�
	/// <summary>
	/// �e��������
	/// </summary>
	void Preparation();

	/// <summary>
	/// ����:�̍U���̏���
	/// </summary>
	/// <param name="playerPosition">�v���C���[�̈ʒu���</param>
	/// <param name="bulletCount">   �e�̔z��ԍ�        </param>
	void BluePreparation(int bulletCount);

	/// <summary>
	/// ����:�Ԃ̍U���̏���
	/// </summary>
	/// <param name="playerPosition">�v���C���[�̈ʒu���</param>
	/// <param name="bulletCount">   �e�̔z��ԍ�        </param>
	void RedPreparation(int bulletCount);
#pragma endregion

#pragma region �U���֐�
	/// <summary>
	/// �������������e����ł��o��
	/// </summary>
	void Shot();

	/// <summary>
	/// ����:�̍U��
	/// </summary>
	/// <param name="wayCount">�@ way���̔Ԗ� </param>
	/// <param name="bulletCount">�e�̔z��ԍ�</param>
	void ShotBlue(int wayCount, int bulletCount);

	/// <summary>
	/// ����:�̍U��
	/// </summary>
	/// <param name="wayCount">   way���̔Ԗ� </param>
	/// <param name="bulletCount">�e�̔z��ԍ�</param>
	void ShotRed(int wayCount, int bulletCount);
#pragma endregion
    /// <summary>
	/// �t���O�̃I�t�ƈʒu�ύX
	/// </summary>
	void BulletReturn();
public:

	Bullet();

#pragma region �v���C���[�ˌ��p�̌Ăяo���֐�

	void Initialize();

	/// <param name="newPlayerPosition">�v���C���[�|�W�V����</param>
	void Update(Vector2 newPlayerPosition);

#pragma endregion

#pragma region �v���C���[�ȊO�ł̎g�p�֐�

	/// <summary>
	/// �v���C���[�ȊO�Ŏg�����߂̒l�������
	/// </summary>
	/// <param name="size">    �e�̃T�C�Y�@</param>
	/// <param name="interval">�U���Ԋu�@�@</param>
	/// <param name="speed">   �e�̑��x�@�@</param>
	/// <param name="wayY">    �e���m�̊Ԋu</param>
	void OtherInitialize(Vector2 size,
		float   interval,
		float   speed,
		float   wayY
	);

	/// <summary>
	/// �v���C���[�ȊO�Œe�����یĂяo��Update
	/// </summary>
	/// <param name="otherPosition"> �������߂̍��W  </param>
	/// <param name="otherAttribute">����(1:�A2:��)</param>
	/// <param name="otherWay">      �e��Way��       </param>
	void OtherUpdate(Vector2 otherPosition,
		int     otherAttribute,
		int     otherWay
	);
#pragma endregion

	Vector2 GetShotPosition(int positionNumber){ return position[positionNumber]; }

	Vector2 GetShotVector(int vectorNumber){ return shotVector[vectorNumber]; }
};
