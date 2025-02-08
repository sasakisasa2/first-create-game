/*************************************************************
��File Name
�@  Player.h(�v���C���[�t�@�C���̒�)
��Function(�@�\)
�@  �v���C���[�֘A�̋@�\�̌Ăяo��
��Info(�ڍ׏��)�@
�@  �v���C���[�֘A��Z�߂ČĂяo���̂�z�肵�쐬
��Other Use(���g�p)
�@  �Ȃ��I�I�I�I�Ăяo�������I�I�I�I�I
��Note(���l)
�@  �v���C���[���g���ۂ͂��̃N���X��
    LoadAsset,Initialize,Update,Render
    �����Ăяo���Ύg����
��Error(���)�@
�@  ���ɂȂ�
**************************************************************/
#pragma once
#include "..\MainProject\Base\pch.h"
#include "..\MainProject\Base\dxtk.h"
#include"..\MainProject\Classes\Player\Movement\Move.h"
#include"..\MainProject\Classes\Player\BulletAttack\BlueBullet.h"
#include"..\MainProject\Classes\Player\PlayerColider\PlayerColider.h"

using namespace DirectX;

class Player 
{
private:
    SimpleMath::Vector2 playerPosition;
    Move    move         ;
    Bullet  attack       ;
    Colider coliderSprite;
public:
    SimpleMath::Vector2 GetPlayerPosition() { return playerPosition; }

    /// <summary>
    /// �v���C���[�̉摜�̏���������sprite
    /// </summary>
    /// <param name="player">   �v���C���[�摜</param>
    /// <param name="Attack">   �U���摜      </param>
    /// <param name="collision">�����蔻��摜</param>
    void LoadAssets(DirectXTK::Sprite& player,
                    DirectXTK::Sprite& Attack, 
                    DirectXTK::Sprite& collision
                    );
    void Initialize();
    void Update();
    /// <summary>
    /// �摜�̕`��
    /// </summary>
    /// <param name="SpriteBatch"></param>
    void Render(DirectX::SpriteBatch* spriteBatch);

};