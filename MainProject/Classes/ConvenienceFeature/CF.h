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

        void TimerReSet();//初期化

        /// <summary>
        /// 指定時間に達したか調べる(毎フレーム呼び出しで使ってください）
        /// 初期化機能はなし
        /// 初期化にはTimerReSet()を使ってください
        /// </summary>
        /// <param name="timeLimit">測定したい時間</param>
        /// <returns>指定時間に達すればtrue,そうでなければfalse</returns>
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
    /// 二点間の距離を測る
    /// </summary>
    /// <param name="position"></param>
    /// <param name="otherPosition"></param>
    /// <returns>二点間の方向</returns>
    SimpleMath::Vector2 DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    float Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// 指定範囲外へ出たかチェック
    /// </summary>
    /// <param name="position">確認したい座標</param>
    /// <param name="width">X座標の範囲</param>
    /// <param name="Height">Y座標の範囲</param>
    /// <returns>出ていればtrue,出ていなければfalse</returns>
    bool PositionRangeOver(SimpleMath::Vector2 position, SimpleMath::Vector2 width, SimpleMath::Vector2 Height);

    /// <summary>
    /// 向かう方向から角度を計算して返す
    /// </summary>
    /// <param name="vector">向かう方向</param>
    /// <param name="angle">現在の角度</param>
    /// <returns>計算後の角度</returns>
    float ChangeVectorToAngle(SimpleMath::Vector2 vector,float angle);

    /// <summary>
    /// 角度から方向を計算して返す
    /// </summary>
    /// <param name="angle">角度</param>
    /// <returns>方向</returns>
    SimpleMath::Vector2 ChangeAngleToVector(float angle);

    /// <summary>
    /// -を+にする
    /// </summary>
    /// <param name="minus">floatの変数</param>
    /// <returns>-の場合は+に、+の場合はそのまま</returns>
    float MinusToPlusf(float minus);

    /// <summary>
    /// -を+にする
    /// </summary>
    /// <param name="minus">intの変数</param>
    /// <returns>-の場合は+に、+の場合はそのまま</returns>
    int MinusToPlus(int minus);
}