/*************************************************************
��File Name
�@  BitAttack.h 
��Function(�@�\)
    AttackBit�ŌĂяo���ꂽBit����ɍU���e����]������
��Info(�ڍ׏��)�@
�@  Bit�݂̂̂ł̎g�p��z�肵�쐬
��Other Use(���g�p)
    ���S���W�A�U���p�^�[��(AttackBit.h�ŏڍ׏�񂪕ς��Ă���܂�)
    ��ݒ�Ŏg�p�\
��Note(���l)
�@�@BitManager�ŏڍאݒ�\
��Error(���)�@
�@  ���̂Ƃ�����Ȃ�
**************************************************************/

#pragma once
#include "..\BitManager\BitManager.h"
using namespace DirectX;
class BitMove;
class BitAttack:public BitManager {
private:
    float attackTheta[BIT_ATTACK_MAX];
    float colorNumber[3]    ;
    bool  colorNumberFlag[3];
public:
    /// <summary>
    /// ������
    /// </summary>
    /// <param name="bitTheta">�@�@�@�@ �����p�x    </param>
    /// <param name="directionRotation">��]����    </param>
    /// <param name="attackDirection">�@�U�������ʒu</param>
    void Initialize(float bitTheta,bool directionRotation,bool attackDirection);

    void Update(Vector2 centerPosition);


};
