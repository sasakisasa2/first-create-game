/*************************************************************
＠File Name
　  PlayerManager.h(プレイヤーファイルの中)
＠Function(機能)
　  プレイヤーの移動やサイズなど詳細な変更をしたい変数を纏めて
    持つclass
＠Info(詳細情報)　
　  画像の描画などもここでやってる
＠Other Use(他使用)
　  プレイヤーのサイズや速度を持っていきたい場合はここにある
    (派生クラスでも呼び出しはできる）
＠Note(備考)
　　プレイヤーのpositionのみMoveから取ってきて
　  継承して使っている(MoveとBullet）
＠Error(問題)　
　  特になし
**************************************************************/
#pragma once
#include<vector>
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
class PlayerManager/*:public SpriteRender*/
{
private:

//********************プレイヤー関係**********************
	int   attribute ;    //プレイヤー属性(1,Blue|2,Red|3,Black)
	int   spriteSize;    //100
	float moveSpeed ;    //300.0f
//********************************************************

//********************プレイヤー関係**********************
	Vector2 playerPosition  ;//プレイヤーの位置
	Vector2 attackSpriteSize;//攻撃弾のサイズ(5対3)
	int     attackWayMax    ;//WAY数 
	float   attackInterval  ;//攻撃間隔
	float   attackDamage    ;//攻撃ダメージ
	float   attackSpeedX    ;//攻撃弾のスピード
	float   attackWayY      ;//弾同士の間隔
//********************************************************
protected:		

#pragma region 派生classのspriteとposition,color
	DirectXTK::Sprite sprite;

	std::vector<SimpleMath::Vector2> position; 
	std::vector<float> angle;
	Color color;
#pragma endregion


#pragma region アクセサ
//***********************プレイヤーアクセサ*************************
#pragma region 属性アクセサ
	void SetAttribute(int getAttribute);

	int GetAttribute();
#pragma endregion
#pragma region プレイヤーサイズアクセサ
	void SetSpriteSize(int size);

	int GetSpriteSize();
#pragma endregion
#pragma region プレイヤースピードアクセサ
	void SetMoveSpeed(float speed);

	float GetMoveSpeed();
#pragma endregion
//******************************************************************

//****************************攻撃関係******************************
#pragma region playerPositionアクセサ
	void SetPosition(Vector2 plPosition);
	Vector2 GetPosition();
#pragma endregion
#pragma region 攻撃サイズアクセサ
	void SetAttackSpriteSize(Vector2 setSize);

	Vector2 GetAttackSpriteSize();
#pragma endregion
#pragma region 攻撃WAY数アクセサ
	void SetAttackWay(int attackWay);
	int  GetAttackWay();
#pragma endregion
#pragma region 攻撃間隔アクセサ
	void SetAttackInterval(float setInterval);

	float GetAttackInterval();
#pragma endregion
#pragma region 攻撃ダメージアクセサ
	void SetAttackDamage(float setDamege);

	float GetAttackDamage();
#pragma endregion
#pragma region 攻撃スピードアクセサ
	void  SetAttackSpeed(float setAtttackSpped);

	float GetAttackSpeed();
#pragma endregion
#pragma region 弾の間隔アクセサ
	void SetAttackWayY(float setAttackWayY);

	float GetAttackWayY();
#pragma endregion

//******************************************************************
#pragma endregion

public: 
	static constexpr int PLAYER_ATTACK_MAX = 100;//弾の全体数
	
	PlayerManager();

	/// <summary>
	/// 画像呼び出し
	/// </summary>
	/// <param name="newSprite">画像の情報を持ったsprite</param>
	void Load  (DirectXTK::Sprite& newSprite);

	/// <summary>
	/// 画像描画
	/// </summary>
	/// <param name="spriteBarch">SpriteBatch   </param>
	/// <param name="count">      何回描画するか</param>
	void Render(DirectX::SpriteBatch* spriteBarch,int count);

	/// <summary>
	/// 座標が画面の外へオーバーしているか確認し、
	/// オーバーしていた場合画面内に戻す
	/// </summary>
	/// <param name="position">判定したい位置情報  </param>
	/// <returns>判定後の位置(何もなければそのまま)</returns>
	SimpleMath::Vector2 PositionScreenOver(SimpleMath::Vector2 position);
};