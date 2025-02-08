//
// MainScene.h
//

#pragma once

#include"../MainProject/Classes/Player/Player.h"
#include"..\Classes\AttackBit\AttackBit.h"
#include "Scene.h"
#include <random>
#include"..\Classes\Enamy\Plant\Plant.h"
#include"..\Classes\Enamy\Boss\Boss.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class MainScene final : public Scene {
public:
	MainScene();
	virtual ~MainScene() { Terminate(); }

	MainScene(MainScene&&) = default;
	MainScene& operator= (MainScene&&) = default;

	MainScene(MainScene const&) = delete;
	MainScene& operator= (MainScene const&) = delete;

	// These are the functions you will implement.
	void Start() override;
	void Initialize() override;
	void LoadAssets() override;

	void Terminate() override;

	void OnDeviceLost() override;
	void OnRestartSound() override;

	NextScene Update(const float deltaTime) override;
	void Render() override;

private:

	Player classPlayer;
	Boss boss;
	static const int BIT_MAX=10;
	AttackBit playerBit[BIT_MAX];
	static const int PLANT_MAX=200;
	Plant enemyPlant[PLANT_MAX];

	//部屋数を増やす
	enum PlayerEnum {
		Player,
		PlayerAttack,
		PlayerBit,
		PlayerBitAttack,
		PlayerCollisionSprite,
	    PlayerCount
	};
	enum EnemyEnum {
		Enemy = PlayerCount,
		EnemyAttack,
		EnemyCount
	};
	enum BossEnum {
		Boss = EnemyCount,
		BossBullet,
		BossInductionBullet,
		BossShotBullet,
		BossBulletFrame,
		BossHP,
		BossHPFrame,
		BossCount
	};
	enum EffectEnum {
		PlayerBitAttackEffect = BossCount,
		PlayerAttackEffect,
		ExclamationMark,
		ExclamationTape,
		EffectCount
	};
	enum SpriteEnum {
		BG= EffectCount,
		TV,
		SpriteFont,
		Test,
		SpriteCount
	};

	std::mt19937 random;
	DirectXTK::DescriptorHeap descHeap;
	DirectXTK::SpriteBatch    sprite_batch_;
	DirectXTK::SpriteFont spriteFont;
	//当たり判定の大きさや位置確認用
	DirectXTK::Sprite test_sprite[10];

#pragma region 当たり判定関数(後クラス化)
	//当たり判定を纏めてる関数
	void CollisionSummarize();
	//--------------プレイヤーの当たり判定------------------
	void PlayerCollision();
	//void PlayerPlantBulletDetection(Rectangle);
	void PlayerBossFrameDetection(SimpleMath::Rectangle);
	void PlayerBossQuantityDetection(SimpleMath::Rectangle);
	void PlayerBossInductionDetection(SimpleMath::Rectangle);
	void PlayerBossShotDetection(SimpleMath::Rectangle);
	//-------------------------------------------------------
	//--------------プレイヤー攻撃の当たり判定---------------
	void PlayerAttackCollision();
	void PlayerAttackBossDetection(SimpleMath::Rectangle, int);
	void PlayerAttackPlantDetection(SimpleMath::Rectangle, int);
	//-------------------------------------------------------
	//--------------プレイヤービット攻撃の当たり判定---------
	void PlayerBitAttackCollision();
	void PlayerBitAttackPlantDetection(SimpleMath::Rectangle, int);
	void PlayerBitAttackBossDetection(SimpleMath::Rectangle, int);
	//-------------------------------------------------------
	//-------------------------------------------------------
#pragma endregion

	DirectXTK::Sprite offOnSprite;
	int wide;//4
	int hyde;//5
	int rectWide;//5120-1280
	int rectHyde;//3600-720
	bool offOnFlag;
	bool onFlag = true;
	double PI = 3.141592;
	float PIAngle = 57.295791;
	//関数でデルタタイムを使う用
	float delTime;

	//const DirectXTK::Sprite* SpriteLoad(const wchar_t* spriteName,int memoryNumber);

	//背景
	DirectXTK::Sprite backGroundSprite;
	SimpleMath::Vector2 backGroundPosition;

#pragma region 攻撃effect(後作成)
	//----------------------------------------------------------	
	////攻撃effect
	////プレイヤー攻撃エフェクト
	////----------------------------------------------------------------
	//void PlayerAttackEffectDrow();
	//DirectXTK::Sprite playerAttackEffectSprite;
	////弾が当たった瞬間の座標を保存
	//SimpleMath::Vector2 playerAttackEffectPosition[PLAYER_ATTACK_MAX];
	////エフェクト表示時間
	//float playerAttackEffectTime[PLAYER_ATTACK_MAX];
	//float playerAttackEffectDrowTime = 0.1;
	////エフェクトを表示
	//bool playerAttackEffectDrowFlag[PLAYER_ATTACK_MAX];
	////エフェクトサイズ
	//int playerAttackEffectSize = 50;
	////-----------------------------------------------------------------


	//-----------------------------------------------------------------
	//攻撃ビット
	//-----------------------------------------------------------------
	//ビット攻撃
	//--------------------------------------------------------------
	//当たり判定の再出現時間
	//void BitAttackCollisionCoolTime();
	//bool bitAttackCollisionDetection[BIT_ATTACK_NUMBER];
	//float bitAttackCoolTime[BIT_ATTACK_NUMBER];
	//float bitAttackCoolTimeEnd = 2;
	//float bitAttackDamage = 5;
	//-------------------------------------------------------------------
	//ビット攻撃エフェクト
	 //------------------------------------------------------------------
	//void BitAttackEffectDrow();
	//DirectXTK::Sprite bitAttackEffectSprite;
	//SimpleMath::Vector2 bitAttackEffectPosition[BIT_ATTACK_NUMBER];
	//bool bitAttackEffectDrowFlag[BIT_ATTACK_NUMBER];
	//float bitAttackEffectTime[BIT_ATTACK_NUMBER];
	////エフェクトが表示される時間
	//float bitAttackEffectDrowTime = 0.3;
	//float bitAttackEffectTransparency[BIT_ATTACK_NUMBER];
	////エフェクトが完全透明になるまでの時間
	//float bitAttackEffectTransparencyTime = 0.3;
	//float bitAttackEffectSize = 50;

	//------------------------------------------------------------------
  //--------------------------------------------------------------------
//----------------------------------------------------------------------
#pragma endregion

	bool isEnemyAttack;
//ボス
	//---------------------------------------------------------------------
		//boss出現
	bool bossFlag = false;

	//敵HP
	DirectXTK::Sprite bossHpSprite[2];
	static constexpr float BOSS_HP = 2000;
	float bossHpLow = BOSS_HP;
	//敵
	DirectXTK::Sprite bossSprite;
	SimpleMath::Vector2 bossPosition;
	//敵攻撃
	float bossAttackStartTime = 0;
	//弾の発射するか(上下）
	bool bossShotFlag = false;

#pragma region 危険信号
	
	DirectXTK::Sprite bossExclamationSprite;
	DirectXTK::Sprite bossExclamationTapeSprite;
	static constexpr float END_SIGNAL_TIME = 2;
	float bossExclamationTime = 0;
	float bossExclamationSignalTime = 0;
	float bossExclamationSignalInterval = 3;
	bool bossExclamationFlag = false;
	bool bossExclamationSignalFlag = false;

#pragma endregion

	//攻撃回数
	static constexpr int BOSS_QUANTITY_ATTACK = 2;
	//弾の数
	static constexpr int BOSS_BULLET_QUANTITY = 1000;

	//球数を攻撃回数で割った数
	int bossBulletQuantity = BOSS_BULLET_QUANTITY / BOSS_QUANTITY_ATTACK;

	//定位置へ位置変更
	float bossPlaceDistance[BOSS_BULLET_QUANTITY];

	//時期狙い弾
	DirectXTK::Sprite bossBulletShotSprite;
	SimpleMath::Vector2 bossBulletShotPosition[BOSS_BULLET_QUANTITY];
	//狙う場所
	SimpleMath::Vector2 bossBulletShotMove[BOSS_BULLET_QUANTITY];

	int bossBulletQuantitySize = 50;

	//時期狙いへ位置変更用
	float bossBulletShotDistance[BOSS_BULLET_QUANTITY];

	//発射を管理する変数
	bool bossBulletShotFlag[BOSS_QUANTITY_ATTACK];

	//弾の定位置までの移動
	bool bossBulletPlaceFlag[BOSS_QUANTITY_ATTACK];

	//弾の発射までの時間
	float bossBulletToShotTime = 2;
	float bossBulletShotTime[BOSS_QUANTITY_ATTACK];

	//弾をどこに置くかの最小値
	float bossQuantityShotPlaceMin = 200;

	//弾と弾の間隔
	float bossQuantityShotPlace = 1280/ bossBulletQuantity;

	//弾をどこの高さで止めるか
	float bossQuantityShotY = 100;

	float bossQuantityPositionMoveSpeed = 1000.0f;

	//弾を撃つ速度
	float bossQuantityShotSpeed = 1000.0f;

	//弾のうち終わりの時間
	float bossBulletEndShotTime =0;


	//誘導弾
	DirectXTK::Sprite bossInductionBulletSprite;
	SimpleMath::Vector2 bossInductionBulletPosition;
	SimpleMath::Vector2 bossInductionBulletMove;
	SimpleMath::Vector2 oldPlayerPosition;
	//目標までの距離
	float bossBulletDistance = 0;
	//弾の大きさを変える秒数の変更
	float bossVolumeTime = 0;
	//大きくする間隔
	float bossVolumeLargeTime = 2;
	//途中で追尾をやめる
	bool bossInductionFlag = false;
	bool oldBossBulletPlaceFlag = true;
	//弾の大きさを変える
	int bossInductionBulletVolume = 1;


	//上下に撃つ弾
	DirectXTK::Sprite bossBulletSprite;
	//敵が動き弾を発射する
	float bossMoveTime = 0;
	float bossBulletTime = 0;
	//弾の数
	static constexpr int BOSS_SHOT_MAX = 60;
	//弾の移動管理
	SimpleMath::Vector2 bossBulletPosition[BOSS_SHOT_MAX];
	//弾の発射
	bool bossBulletFlag[BOSS_SHOT_MAX];

	//電流デスマッチ
	bool bossAttackFrameFlag = false;
	bool bossAttackFramePlaceFlag = false;
	static constexpr int BOSS_FRAME_MAX = 400;
	DirectXTK::Sprite bossFrameSprite;
	SimpleMath::Vector2 frame_pos[BOSS_FRAME_MAX];
	float frame_move_x[BOSS_FRAME_MAX];
	float frame_move_y[BOSS_FRAME_MAX];
	float frame_quarter = BOSS_FRAME_MAX / 4;
	float frame_eighth = BOSS_FRAME_MAX / 4 / 2;
	float frame_size = 150;

	//-------------------------------------------------------------------------------



	DirectXTK::Sound hit_se_;
	DirectXTK::SoundInstance hit_se_instance;
};