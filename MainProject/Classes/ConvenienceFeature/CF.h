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
        /// 時間の初期化
        /// </summary>
        void TimerReSet();

        /// <summary>
        /// 指定時間に達したか調べる(毎フレーム呼び出しで使ってください）
        /// 初期化機能はなし
        /// 初期化にはTimerReSet()を使ってください
        /// </summary>
        /// <param name="timeLimit">測定したい時間</param>
        /// <returns>指定時間に達すればtrue,そうでなければfalse</returns>
        bool TimeMeasurement(float timeLimit);
	};

    /// <summary>
    /// 指定した範囲内でランダムな値を返す(float)
    /// </summary>
    /// <param name="randomRange">範囲</param>
    /// <returns>範囲内の値</returns>
    float SetRandomf(SimpleMath::Vector2 randamRange);

    /// <summary>
    /// 指定した範囲内でランダムな値を返す(int)
    /// </summary>
    /// <param name="randomRange">範囲</param>
    /// <returns>範囲内の値</returns>
    int   SetRandom(SimpleMath::Vector2 randamRange);

    /// <summary>
    /// 二点間の方向を計算する
    /// </summary>
    /// <param name="positionOne"></param>
    /// <param name="positionTwo"></param>
    /// <returns>二点間の方向</returns>
    SimpleMath::Vector2 DistanceCount(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// 二点間の距離を測る
    /// </summary>
    /// <param name="positionOne"></param>
    /// <param name="positionTwo"></param>
    /// <returns>二点間の距離</returns>
    float Distance(SimpleMath::Vector2 positionOne, SimpleMath::Vector2 positionTwo);

    /// <summary>
    /// 指定範囲外へ出たかチェック
    /// </summary>
    /// <param name="position">確認したい座標</param>
    /// <param name="width">   X座標の範囲   </param>
    /// <param name="Height">  Y座標の範囲   </param>
    /// <returns>出ていればtrue,出ていなければfalse</returns>
    bool PositionRangeOver(SimpleMath::Vector2 position, SimpleMath::Vector2 width, SimpleMath::Vector2 Height);

    /// <summary>
    /// 向かう方向から角度を計算して返す
    /// </summary>
    /// <param name="vector">向かう方向</param>
    /// <param name="angle"> 現在の角度</param>
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

    /// <summary>
    /// 中心点から角への角度を求める
    /// </summary>
    /// <param name="size">サイズ</param>
    /// <returns>角への角度</returns>
    SimpleMath::Vector2 RectangleCornerAngle(SimpleMath::Vector2 size);

    /// <summary>
    /// 角度を参照し座標を動かす関数
    /// </summary>
    /// <param name="theta">		 角度(真下が0度,真上が180度)</param>
    /// <param name="centerPosition">中心座標					</param>
    /// <param name="distance">		 中心座標からの距離			</param>
    /// <returns>					 動かした後の座標			</returns>
    SimpleMath::Vector2 SquareMovement(float theta, SimpleMath::Vector2 position, float distance);

}