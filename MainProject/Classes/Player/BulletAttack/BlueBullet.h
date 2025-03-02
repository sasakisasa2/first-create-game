/*************************************************************
＠File Name
　  Bullet.h <- 後書き換え(プレイヤーファイルのBulletAttackファイルの中)
＠Function(機能)
　  弾の発射
＠Info(詳細情報)　
　  プレイヤーで使用を想定し作成
＠Other Use(他使用)
　  OtherUpdateとOtherInitializeを呼び出しで使用可
＠Note(備考)
　　プレイヤーで使う時はPlayerManagerで詳細設定可能
＠Error(問題)　
　  今のところ問題なし
**************************************************************/
#pragma once
#include"../MainProject/Classes/Player/PlayerManager.h"


using namespace DirectX;

using namespace SimpleMath;

class Bullet : public PlayerManager
{
private:
	CF::Timer timerCount;

	Vector2 shotVector[PLAYER_ATTACK_MAX];//撃つ方向
	int     wayHalf;					  //WAY数の半分
	bool    attackFlag[PLAYER_ATTACK_MAX];//攻撃するかどうか

#pragma region 攻撃準備用関数
	/// <summary>
	/// 弾を撃つ準備
	/// </summary>
	void Preparation();

	/// <summary>
	/// 属性:青の攻撃の準備
	/// </summary>
	/// <param name="playerPosition">プレイヤーの位置情報</param>
	/// <param name="bulletCount">   弾の配列番号        </param>
	void BluePreparation(int bulletCount);

	/// <summary>
	/// 属性:赤の攻撃の準備
	/// </summary>
	/// <param name="playerPosition">プレイヤーの位置情報</param>
	/// <param name="bulletCount">   弾の配列番号        </param>
	void RedPreparation(int bulletCount);
#pragma endregion

#pragma region 攻撃関数
	/// <summary>
	/// 準備完了した弾から打ち出す
	/// </summary>
	void Shot();

	/// <summary>
	/// 属性:青の攻撃
	/// </summary>
	/// <param name="wayCount">　 way数の番目 </param>
	/// <param name="bulletCount">弾の配列番号</param>
	void ShotBlue(int wayCount, int bulletCount);

	/// <summary>
	/// 属性:青の攻撃
	/// </summary>
	/// <param name="wayCount">   way数の番目 </param>
	/// <param name="bulletCount">弾の配列番号</param>
	void ShotRed(int wayCount, int bulletCount);
#pragma endregion
    /// <summary>
	/// フラグのオフと位置変更
	/// </summary>
	void BulletReturn();
public:

	Bullet();

#pragma region プレイヤー射撃用の呼び出し関数

	void Initialize();

	/// <param name="newPlayerPosition">プレイヤーポジション</param>
	void Update(Vector2 newPlayerPosition);

#pragma endregion

#pragma region プレイヤー以外での使用関数

	/// <summary>
	/// プレイヤー以外で使うための値代入処理
	/// </summary>
	/// <param name="size">    弾のサイズ　</param>
	/// <param name="interval">攻撃間隔　　</param>
	/// <param name="speed">   弾の速度　　</param>
	/// <param name="wayY">    弾同士の間隔</param>
	void OtherInitialize(Vector2 size,
		float   interval,
		float   speed,
		float   wayY
	);

	/// <summary>
	/// プレイヤー以外で弾を撃つ際呼び出すUpdate
	/// </summary>
	/// <param name="otherPosition"> 撃ち初めの座標  </param>
	/// <param name="otherAttribute">属性(1:青、2:赤)</param>
	/// <param name="otherWay">      弾のWay数       </param>
	void OtherUpdate(Vector2 otherPosition,
		int     otherAttribute,
		int     otherWay
	);
#pragma endregion

	Vector2 GetShotPosition(int positionNumber){ return position[positionNumber]; }

	Vector2 GetShotVector(int vectorNumber){ return shotVector[vectorNumber]; }
};
