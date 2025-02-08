/*************************************************************
��File Name
�@  AttackBit.h <- �㏑������(�v���C���[�t�@�C����BulletAttack�t�@�C���̒�)
��Function(�@�\)
�@  Bit�ƍU����Z�߂ČĂяo��
    �w�肳�ꂽ���W����^�����玞�v����360������ɉ�]�ړ�
��Info(�ڍ׏��)�@
�@  �v���C���[�ȊO�ł��g�p�ł���悤�쐬
��Other Use(���g�p)
    Initialize�ň����ɏ����p�x�A��]����(false�Ŏ��v���,true�Ŕ����v���)�A�U���e�̋Ȃ�������w��)
    Update�Œǔ������������W���w��Ŏg�p�\
��Note(���l)
�@�@BitManager�ŏڍאݒ�\
��Error(���)�@
�@  ���̂Ƃ�����Ȃ�
**************************************************************/

#pragma once
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include"..\AttackBit\BitAttack\BitAttack.h"
#include"..\AttackBit\BitMove\BitMove.h"
using namespace DirectX;
using namespace SimpleMath;
class AttackBit{
private:
    BitMove   move;
    BitAttack attack;
public:
    void Load(DirectXTK::Sprite& bitSprite,DirectXTK::Sprite& bitAttackSprite);
    /// <summary>
    /// ������
    /// </summary>
    /// <param name="bitTheta">�@�@�@�@ �����p�x    </param>
    /// <param name="directionRotation">��]����    </param>
    /// <param name="attackDirection">�@�U�������ʒu</param>
    void Initialize(float theta, bool directionRotation, bool attackDirection);


    void Update(Vector2 centerPosition);

    void Render(DirectX::SpriteBatch* spriteBatch);
};
