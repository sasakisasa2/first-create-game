/*************************************************************
＠File Name
　  BitMove.h
＠Function(機能)
　  中心座標から円を作りその円状を移動
＠Info(詳細情報)　
　  回転移動のみ(後追加するかも）
＠Other Use(他使用)
    中心座標、初期角度、回転方向を指定で使用可能(詳細はAttackBit.hに書いてます
＠Note(備考)
　　BitManagerで詳細設定可能
＠Error(問題)　
　  今のところ問題なし
**************************************************************/

#pragma once
#include"..\BitManager\BitManager.h"
using namespace DirectX;
using namespace SimpleMath;
class BitMove:public BitManager {
private:

    float bitTheta;//ビットの角度

public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="bitTheta">　　　　  初期角度 </param>
    /// <param name="directionRotation"> 回転方向 </param>
    void Initialize(float theta, bool directionRotation);

    void Update(Vector2 orderPosition);
};
