/*************************************************************
＠File Name
　  PlayerColider.h(プレイヤーファイルのPlayerColiderファイル中)
＠Function(機能)
　  当たり判定の可視化
＠Info(詳細情報)　
　  当たり判定と同じ位置に画像を出すのを想定し作成
＠Other Use(他使用)
　  サイズは変更不可だが、移す位置の変更は可能
＠Note(備考)
　  特になし
＠Error(問題)　
　  特になし
**************************************************************/
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
#include"..\MainProject\Classes\Player\PlayerManager.h"

class Colider :public PlayerManager
{
private:

public:
	void Initialize();
	void Update(SimpleMath::Vector2 playerPosition);
};