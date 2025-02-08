/*************************************************************
＠File Name
　  Player.h(プレイヤーファイルの中)
＠Function(機能)
　  プレイヤー関連の機能の呼び出し
＠Info(詳細情報)　
　  プレイヤー関連を纏めて呼び出すのを想定し作成
＠Other Use(他使用)
　  なし！！！！呼び出しだけ！！！！！
＠Note(備考)
　  プレイヤーを使う際はこのクラスの
    LoadAsset,Initialize,Update,Render
    さえ呼び出せば使える
＠Error(問題)　
　  特になし
**************************************************************/
#pragma once
#include "..\MainProject\Base\pch.h"
#include "..\MainProject\Base\dxtk.h"
#include"..\MainProject\Classes\Player\Movement\Move.h"
#include"..\MainProject\Classes\Player\BulletAttack\BlueBullet.h"
#include"..\MainProject\Classes\Player\PlayerColider\PlayerColider.h"

using namespace DirectX;

class Player 
{
private:
    SimpleMath::Vector2 playerPosition;
    Move    move         ;
    Bullet  attack       ;
    Colider coliderSprite;
public:
    SimpleMath::Vector2 GetPlayerPosition() { return playerPosition; }

    /// <summary>
    /// プレイヤーの画像の情報を持ったsprite
    /// </summary>
    /// <param name="player">   プレイヤー画像</param>
    /// <param name="Attack">   攻撃画像      </param>
    /// <param name="collision">当たり判定画像</param>
    void LoadAssets(DirectXTK::Sprite& player,
                    DirectXTK::Sprite& Attack, 
                    DirectXTK::Sprite& collision
                    );
    void Initialize();
    void Update();
    /// <summary>
    /// 画像の描画
    /// </summary>
    /// <param name="SpriteBatch"></param>
    void Render(DirectX::SpriteBatch* spriteBatch);

};