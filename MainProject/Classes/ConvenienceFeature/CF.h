#pragma once
#include<random>
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"

using namespace DirectX;

namespace CF 
{
	class Timer
    {
    private:
        float time;
    public:
        Timer();

        /// <summary>
        /// ���Ԃ̏�����
        /// </summary>
        void TimerReSet();

        /// <summary>
        /// �w�莞�ԂɒB���������ׂ�(���t���[���Ăяo���Ŏg���Ă��������j
        /// �������@�\�͂Ȃ�
        /// �������ɂ�TimerReSet()���g���Ă�������
        /// </summary>
        /// <param name="timeLimit">���肵��������</param>
        /// <returns>�w�莞�ԂɒB�����true,�����łȂ����false</returns>
        bool TimeMeasurement(float timeLimit);
	};

    /// <summary>
    /// �w�肵���͈͓��Ń����_���Ȓl��Ԃ�(float)
    /// </summary>
    /// <param name="randomRange">�͈�</param>
    /// <returns>�͈͓��̒l</returns>
    float SetRandomf(SimpleMath::Vector2 randamRange);

    /// <summary>
    /// �w�肵���͈͓��Ń����_���Ȓl��Ԃ�(int)
    /// </summary>
    /// <param name="randomRange">�͈�</param>
    /// <returns>�͈͓��̒l</returns>
    int   SetRandom(SimpleMath::Vector2 randamRange);

    /// <summary>
    /// ��_�Ԃ̕������v�Z����
    /// </summary>
    /// <param name="positionOne"></param>
    /// <param name="positionTwo"></param>
    /// <returns>��_�Ԃ̕���</returns>
    SimpleMath::Vector2 DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// ��_�Ԃ̋����𑪂�
    /// </summary>
    /// <param name="positionOne"></param>
    /// <param name="positionTwo"></param>
    /// <returns>��_�Ԃ̋���</returns>
    float Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// �w��͈͊O�֏o�����`�F�b�N
    /// </summary>
    /// <param name="position">�m�F���������W</param>
    /// <param name="width">   X���W�͈̔�   </param>
    /// <param name="Height">  Y���W�͈̔�   </param>
    /// <returns>�o�Ă����true,�o�Ă��Ȃ����false</returns>
    bool PositionRangeOver(SimpleMath::Vector2 position, SimpleMath::Vector2 width, SimpleMath::Vector2 Height);

    /// <summary>
    /// ��������������p�x���v�Z���ĕԂ�
    /// </summary>
    /// <param name="vector">����������</param>
    /// <param name="angle"> ���݂̊p�x</param>
    /// <returns>�v�Z��̊p�x</returns>
    float ChangeVectorToAngle(SimpleMath::Vector2 vector,float angle);

    /// <summary>
    /// �p�x����������v�Z���ĕԂ�
    /// </summary>
    /// <param name="angle">�p�x</param>
    /// <returns>����</returns>
    SimpleMath::Vector2 ChangeAngleToVector(float angle);

    /// <summary>
    /// -��+�ɂ���
    /// </summary>
    /// <param name="minus">float�̕ϐ�</param>
    /// <returns>-�̏ꍇ��+�ɁA+�̏ꍇ�͂��̂܂�</returns>
    float MinusToPlusf(float minus);

    /// <summary>
    /// -��+�ɂ���
    /// </summary>
    /// <param name="minus">int�̕ϐ�</param>
    /// <returns>-�̏ꍇ��+�ɁA+�̏ꍇ�͂��̂܂�</returns>
    int MinusToPlus(int minus);

    /// <summary>
    /// ���S�_����p�ւ̊p�x�����߂�
    /// </summary>
    /// <param name="size">�T�C�Y</param>
    /// <returns>�p�ւ̊p�x</returns>
    SimpleMath::Vector2 RectangleCornerAngle(SimpleMath::Vector2 size);

    /// <summary>
    /// �p�x���Q�Ƃ����W�𓮂����֐�
    /// </summary>
    /// <param name="theta">		 �p�x(�^����0�x,�^�オ180�x)</param>
    /// <param name="centerPosition">���S���W					</param>
    /// <param name="distance">		 ���S���W����̋���			</param>
    /// <returns>					 ����������̍��W			</returns>
    SimpleMath::Vector2 SquareMovement(float theta, SimpleMath::Vector2 position, float distance);

}