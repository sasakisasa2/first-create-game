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

        void TimerReSet();//������

        /// <summary>
        /// �w�莞�ԂɒB���������ׂ�(���t���[���Ăяo���Ŏg���Ă��������j
        /// �������@�\�͂Ȃ�
        /// �������ɂ�TimerReSet()���g���Ă�������
        /// </summary>
        /// <param name="timeLimit">���肵��������</param>
        /// <returns>�w�莞�ԂɒB�����true,�����łȂ����false</returns>
        bool TimeMeasurement(float timeLimit);
	};
    class Random 
    {
    private:
        std::mt19937 random;
    public:
        Random();
        float SetRandomf(SimpleMath::Vector2 randamRange);
        int SetRandom(SimpleMath::Vector2 randamRange);
    };

    /// <summary>
    /// ��_�Ԃ̋����𑪂�
    /// </summary>
    /// <param name="position"></param>
    /// <param name="otherPosition"></param>
    /// <returns>��_�Ԃ̕���</returns>
    SimpleMath::Vector2 DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    float Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// �w��͈͊O�֏o�����`�F�b�N
    /// </summary>
    /// <param name="position">�m�F���������W</param>
    /// <param name="width">X���W�͈̔�</param>
    /// <param name="Height">Y���W�͈̔�</param>
    /// <returns>�o�Ă����true,�o�Ă��Ȃ����false</returns>
    bool PositionRengeOver(SimpleMath::Vector2 position, SimpleMath::Vector2 width, SimpleMath::Vector2 Height);
}