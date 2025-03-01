/*************************************************************
＠File Name
　  BitManager.h
＠Function(機能)
　  Bitの攻撃と移動の変数の詳細設定
＠Info(詳細情報)　
　  Bitに関連する共通の変数や詳細な設定をできる変数をを纏めて管理するように作成
＠Other Use(他使用)
	ほしい変数のgetter関数などを呼び出してください
＠Note(備考)
　　ビットの座標や動かしてる変数は動かしているクラスから呼び出してください
＠Error(問題)　
　  今のところ問題なし
**************************************************************/

#pragma once
#include<vector>
#include"..\MainProject\Base\pch.h"
#include"..\MainProject\Base\dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
using namespace CF;

class BitManager 
{
private:

#pragma region 共通
	float rotationAngle;
#pragma endregion
#pragma region BitMove
	Vector2 spriteSize;//ビットの大きさ
	float   distance;  //プレイヤーとビットの距離
	float   omega;     //ビットの回転幅
#pragma endregion
#pragma region BitAttack
	Vector2 attackSpriteSize;//ビットの攻撃の弾の大きさ
	Vector2 attackSpace;     //弾同士の距離
#pragma endregion

protected:

#pragma region 派生クラスの画像描画用
	std::vector<Vector2> position;
	DirectXTK::Sprite    sprite  ;
	std::vector<Color>   color   ;
	std::vector<float>   angle   ;
#pragma endregion

	BitManager();

#pragma region アクセサ
#pragma region 共通アクセサ
#pragma region 角度設定用アクセサ
	void  SetRotationAngle(float setAngle);
	float GetRotationAngle();
#pragma endregion
#pragma endregion
#pragma region BitMoveのアクセサ
#pragma region 画像サイズ
	void    SetSpriteSize(Vector2 setSize);
	Vector2 GetSpriteSize();
#pragma endregion
#pragma region 中心座標からBitの距離
	void    SetDistance(float setDistance);
	float   GetDistance();
#pragma endregion
#pragma region 回転速度
	void    SetOmega(float omega);
	float   GetOmega();
#pragma endregion
#pragma endregion
#pragma region BitAttackのアクセサ
#pragma region ビットの攻撃サイズ
	void    SetAttackSpriteSize(Vector2 setAttackSize);
	Vector2 GetAttackSpriteSize();
#pragma endregion
#pragma region ビットの攻撃弾の間隔
	void    SetAttackSpace(Vector2 setAttackSpace);
	Vector2 GetAttackSpace();
#pragma endregion
#pragma endregion
#pragma endregion

public:

	static constexpr int BIT_ATTACK_MAX = 100;//攻撃の最大数

	void Load(DirectXTK::Sprite& newSprite);
	/// <summary>
	/// 画像描画
	/// </summary>
	/// <param name="spriteBarch">SpriteBatch   </param>
	/// <param name="count">      何回描画するか</param>
	void Render(DirectX::SpriteBatch* spriteBarch, int count);
};