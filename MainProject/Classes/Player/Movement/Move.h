/*************************************************************
��File Name
�@  Move.h(�v���C���[file�́APlayerMovementfile�̒�)
��Function(�@�\)
�@  WASD�Ɓ��������̃L�[�{�[�h����ł̈ړ��N���X
��Info(�ڍ׏��)�@ 
�@  �v���C���[�Ŏg�p��z�肵�쐬�A
    �����ʒuVector2(300,300)�œ��͂��������ɓ�����
��Other Use(���g�p)
�@  �����ʒu�ύX�s�����ړ��͎g�p�\
    �ړ�������̒l��ReturnPlayerPosition()�Ŏ擾�\
��Note(���l)
�@  PlayerManager�ŏڍאݒ�\
��Error(���)�@
�@  �����ʒu�̕ύX��PlayerManager�ł̂݉�
    �ړ����@��WASD�Ɓ��������̂�
**************************************************************/
#pragma once
#include"..\MainProject\Classes\Player\PlayerManager.h"

using namespace DirectX;
using namespace SimpleMath;
class Move:public PlayerManager
{
public:
    void Initialize();
    void Update();

    float   GetAngle()  { return angle; }
    Vector2 GetVector() { return moveVector; }
    Vector2 GetPlayerPosition() { return position[0];};
private:
    Vector2 moveVector;
    Vector2 MoveWASD(Vector2) ;
    Vector2 MoveArrow(Vector2);
#pragma region �������邩��
////���G�_�b�V��
//// ���G�_�b�V���̖��G����{�ʏ�_�b�V���ւ̕ύX�̊Ǘ�
//bool invincibleMove = false;
////���G�_�b�V������
//float moveTime = 0;
////���G�摜
//DirectXTK::Sprite invincibleSprite;
#pragma endregion
};
