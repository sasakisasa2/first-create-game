/*************************************************************
��File Name
�@  PlayerColider.h(�v���C���[�t�@�C����PlayerColider�t�@�C����)
��Function(�@�\)
�@  �����蔻��̉���
��Info(�ڍ׏��)�@
�@  �����蔻��Ɠ����ʒu�ɉ摜���o���̂�z�肵�쐬
��Other Use(���g�p)
�@  �T�C�Y�͕ύX�s�����A�ڂ��ʒu�̕ύX�͉\
��Note(���l)
�@  ���ɂȂ�
��Error(���)�@
�@  ���ɂȂ�
**************************************************************/
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Classes\Player\PlayerManager.h"

class Colider :public PlayerManager
{
private:

public:
	void Initialize();
	void Update(SimpleMath::Vector2 playerPosition);
};