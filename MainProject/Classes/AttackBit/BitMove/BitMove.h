/*************************************************************
��File Name
�@  BitMove.h
��Function(�@�\)
�@  ���S���W����~����肻�̉~����ړ�
��Info(�ڍ׏��)�@
�@  ��]�ړ��̂�(��ǉ����邩���j
��Other Use(���g�p)
    ���S���W�A�����p�x�A��]�������w��Ŏg�p�\(�ڍׂ�AttackBit.h�ɏ����Ă܂�
��Note(���l)
�@�@BitManager�ŏڍאݒ�\
��Error(���)�@
�@  ���̂Ƃ�����Ȃ�
**************************************************************/

#pragma once
#include"..\BitManager\BitManager.h"
using namespace DirectX;
using namespace SimpleMath;
class BitMove:public BitManager {
private:

    float bitTheta;//�r�b�g�̊p�x

public:
    /// <summary>
    /// ������
    /// </summary>
    /// <param name="bitTheta">�@�@�@�@  �����p�x </param>
    /// <param name="directionRotation"> ��]���� </param>
    void Initialize(float theta, bool directionRotation);

    void Update(Vector2 orderPosition);
};
