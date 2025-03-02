/*************************************************************
＠File Name
　  Move.h(プレイヤーfileの、PlayerMovementfileの中)
＠Function(機能)
　  WASDと↑←↓→のキーボード操作での移動クラス
＠Info(詳細情報)　 
　  プレイヤーで使用を想定し作成、
    初期位置Vector2(300,300)で入力した方向に動かす
＠Other Use(他使用)
　  初期位置変更不可だが移動は使用可能
    移動した後の値はReturnPlayerPosition()で取得可能
＠Note(備考)
　  PlayerManagerで詳細設定可能
＠Error(問題)　
　  初期位置の変更がPlayerManagerでのみ可
    移動方法がWASDと↑←↓→のみ
**************************************************************/
#pragma once
#include"..\MainProject\Classes\Player\PlayerManager.h"

using namespace DirectX;
using namespace SimpleMath;
class Move:public PlayerManager
{
public:
    void Initialize();
    void Update();

    float   GetAngle()  { return angle; }
    Vector2 GetVector() { return moveVector; }
    Vector2 GetPlayerPosition() { return position[0];};
private:
    Vector2 moveVector;
    Vector2 MoveWASD(Vector2) ;
    Vector2 MoveArrow(Vector2);
#pragma region 実装するかも
////無敵ダッシュ
//// 無敵ダッシュの無敵判定＋通常ダッシュへの変更の管理
//bool invincibleMove = false;
////無敵ダッシュ時間
//float moveTime = 0;
////無敵画像
//DirectXTK::Sprite invincibleSprite;
#pragma endregion
};
