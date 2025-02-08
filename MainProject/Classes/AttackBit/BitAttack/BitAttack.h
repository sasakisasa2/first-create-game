/*************************************************************
＠File Name
　  BitAttack.h 
＠Function(機能)
    AttackBitで呼び出されたBitを基準に攻撃弾を回転させる
＠Info(詳細情報)　
　  Bitののみでの使用を想定し作成
＠Other Use(他使用)
    中心座標、攻撃パターン(AttackBit.hで詳細情報が変えてあります)
    を設定で使用可能
＠Note(備考)
　　BitManagerで詳細設定可能
＠Error(問題)　
　  今のところ問題なし
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
    /// 初期化
    /// </summary>
    /// <param name="bitTheta">　　　　 初期角度    </param>
    /// <param name="directionRotation">回転方向    </param>
    /// <param name="attackDirection">　攻撃初期位置</param>
    void Initialize(float bitTheta,bool directionRotation,bool attackDirection);

    void Update(Vector2 centerPosition);


};
