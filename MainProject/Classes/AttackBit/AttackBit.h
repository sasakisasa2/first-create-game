/*************************************************************
＠File Name
　  AttackBit.h <- 後書き換え(プレイヤーファイルのBulletAttackファイルの中)
＠Function(機能)
　  Bitと攻撃を纏めて呼び出す
    指定された座標から真下から時計回りに360°を基準に回転移動
＠Info(詳細情報)　
　  プレイヤー以外でも使用できるよう作成
＠Other Use(他使用)
    Initializeで引数に初期角度、回転方向(falseで時計回り,trueで反時計回り)、攻撃弾の曲がる方向指定)
    Updateで追尾させたい座標を指定で使用可能
＠Note(備考)
　　BitManagerで詳細設定可能
＠Error(問題)　
　  今のところ問題なし
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
    /// 初期化
    /// </summary>
    /// <param name="bitTheta">　　　　 初期角度    </param>
    /// <param name="directionRotation">回転方向    </param>
    /// <param name="attackDirection">　攻撃初期位置</param>
    void Initialize(float theta, bool directionRotation, bool attackDirection);


    void Update(Vector2 centerPosition);

    void Render(DirectX::SpriteBatch* spriteBatch);
};
