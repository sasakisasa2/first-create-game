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

	//�������𑝂₷
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
	//�����蔻��̑傫����ʒu�m�F�p
	DirectXTK::Sprite test_sprite[10];

#pragma region �����蔻��֐�(��N���X��)
	//�����蔻���Z�߂Ă�֐�
	void CollisionSummarize();
	//--------------�v���C���[�̓����蔻��------------------
	void PlayerCollision();
	//void PlayerPlantBulletDetection(Rectangle);
	void PlayerBossFrameDetection(SimpleMath::Rectangle);
	void PlayerBossQuantityDetection(SimpleMath::Rectangle);
	void PlayerBossInductionDetection(SimpleMath::Rectangle);
	void PlayerBossShotDetection(SimpleMath::Rectangle);
	//-------------------------------------------------------
	//--------------�v���C���[�U���̓����蔻��---------------
	void PlayerAttackCollision();
	void PlayerAttackBossDetection(SimpleMath::Rectangle, int);
	void PlayerAttackPlantDetection(SimpleMath::Rectangle, int);
	//-------------------------------------------------------
	//--------------�v���C���[�r�b�g�U���̓����蔻��---------
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
	//�֐��Ńf���^�^�C�����g���p
	float delTime;

	//const DirectXTK::Sprite* SpriteLoad(const wchar_t* spriteName,int memoryNumber);

	//�w�i
	DirectXTK::Sprite backGroundSprite;
	SimpleMath::Vector2 backGroundPosition;

#pragma region �U��effect(��쐬)
	//----------------------------------------------------------	
	////�U��effect
	////�v���C���[�U���G�t�F�N�g
	////----------------------------------------------------------------
	//void PlayerAttackEffectDrow();
	//DirectXTK::Sprite playerAttackEffectSprite;
	////�e�����������u�Ԃ̍��W��ۑ�
	//SimpleMath::Vector2 playerAttackEffectPosition[PLAYER_ATTACK_MAX];
	////�G�t�F�N�g�\������
	//float playerAttackEffectTime[PLAYER_ATTACK_MAX];
	//float playerAttackEffectDrowTime = 0.1;
	////�G�t�F�N�g��\��
	//bool playerAttackEffectDrowFlag[PLAYER_ATTACK_MAX];
	////�G�t�F�N�g�T�C�Y
	//int playerAttackEffectSize = 50;
	////-----------------------------------------------------------------


	//-----------------------------------------------------------------
	//�U���r�b�g
	//-----------------------------------------------------------------
	//�r�b�g�U��
	//--------------------------------------------------------------
	//�����蔻��̍ďo������
	//void BitAttackCollisionCoolTime();
	//bool bitAttackCollisionDetection[BIT_ATTACK_NUMBER];
	//float bitAttackCoolTime[BIT_ATTACK_NUMBER];
	//float bitAttackCoolTimeEnd = 2;
	//float bitAttackDamage = 5;
	//-------------------------------------------------------------------
	//�r�b�g�U���G�t�F�N�g
	 //------------------------------------------------------------------
	//void BitAttackEffectDrow();
	//DirectXTK::Sprite bitAttackEffectSprite;
	//SimpleMath::Vector2 bitAttackEffectPosition[BIT_ATTACK_NUMBER];
	//bool bitAttackEffectDrowFlag[BIT_ATTACK_NUMBER];
	//float bitAttackEffectTime[BIT_ATTACK_NUMBER];
	////�G�t�F�N�g���\������鎞��
	//float bitAttackEffectDrowTime = 0.3;
	//float bitAttackEffectTransparency[BIT_ATTACK_NUMBER];
	////�G�t�F�N�g�����S�����ɂȂ�܂ł̎���
	//float bitAttackEffectTransparencyTime = 0.3;
	//float bitAttackEffectSize = 50;

	//------------------------------------------------------------------
  //--------------------------------------------------------------------
//----------------------------------------------------------------------
#pragma endregion

	bool isEnemyAttack;
//�{�X
	//---------------------------------------------------------------------
		//boss�o��
	bool bossFlag = false;

	//�GHP
	DirectXTK::Sprite bossHpSprite[2];
	static constexpr float BOSS_HP = 2000;
	float bossHpLow = BOSS_HP;
	//�G
	DirectXTK::Sprite bossSprite;
	SimpleMath::Vector2 bossPosition;
	//�G�U��
	float bossAttackStartTime = 0;
	//�e�̔��˂��邩(�㉺�j
	bool bossShotFlag = false;

#pragma region �댯�M��
	
	DirectXTK::Sprite bossExclamationSprite;
	DirectXTK::Sprite bossExclamationTapeSprite;
	static constexpr float END_SIGNAL_TIME = 2;
	float bossExclamationTime = 0;
	float bossExclamationSignalTime = 0;
	float bossExclamationSignalInterval = 3;
	bool bossExclamationFlag = false;
	bool bossExclamationSignalFlag = false;

#pragma endregion

	//�U����
	static constexpr int BOSS_QUANTITY_ATTACK = 2;
	//�e�̐�
	static constexpr int BOSS_BULLET_QUANTITY = 1000;

	//�������U���񐔂Ŋ�������
	int bossBulletQuantity = BOSS_BULLET_QUANTITY / BOSS_QUANTITY_ATTACK;

	//��ʒu�ֈʒu�ύX
	float bossPlaceDistance[BOSS_BULLET_QUANTITY];

	//�����_���e
	DirectXTK::Sprite bossBulletShotSprite;
	SimpleMath::Vector2 bossBulletShotPosition[BOSS_BULLET_QUANTITY];
	//�_���ꏊ
	SimpleMath::Vector2 bossBulletShotMove[BOSS_BULLET_QUANTITY];

	int bossBulletQuantitySize = 50;

	//�����_���ֈʒu�ύX�p
	float bossBulletShotDistance[BOSS_BULLET_QUANTITY];

	//���˂��Ǘ�����ϐ�
	bool bossBulletShotFlag[BOSS_QUANTITY_ATTACK];

	//�e�̒�ʒu�܂ł̈ړ�
	bool bossBulletPlaceFlag[BOSS_QUANTITY_ATTACK];

	//�e�̔��˂܂ł̎���
	float bossBulletToShotTime = 2;
	float bossBulletShotTime[BOSS_QUANTITY_ATTACK];

	//�e���ǂ��ɒu�����̍ŏ��l
	float bossQuantityShotPlaceMin = 200;

	//�e�ƒe�̊Ԋu
	float bossQuantityShotPlace = 1280/ bossBulletQuantity;

	//�e���ǂ��̍����Ŏ~�߂邩
	float bossQuantityShotY = 100;

	float bossQuantityPositionMoveSpeed = 1000.0f;

	//�e�������x
	float bossQuantityShotSpeed = 1000.0f;

	//�e�̂����I���̎���
	float bossBulletEndShotTime =0;


	//�U���e
	DirectXTK::Sprite bossInductionBulletSprite;
	SimpleMath::Vector2 bossInductionBulletPosition;
	SimpleMath::Vector2 bossInductionBulletMove;
	SimpleMath::Vector2 oldPlayerPosition;
	//�ڕW�܂ł̋���
	float bossBulletDistance = 0;
	//�e�̑傫����ς���b���̕ύX
	float bossVolumeTime = 0;
	//�傫������Ԋu
	float bossVolumeLargeTime = 2;
	//�r���Œǔ�����߂�
	bool bossInductionFlag = false;
	bool oldBossBulletPlaceFlag = true;
	//�e�̑傫����ς���
	int bossInductionBulletVolume = 1;


	//�㉺�Ɍ��e
	DirectXTK::Sprite bossBulletSprite;
	//�G�������e�𔭎˂���
	float bossMoveTime = 0;
	float bossBulletTime = 0;
	//�e�̐�
	static constexpr int BOSS_SHOT_MAX = 60;
	//�e�̈ړ��Ǘ�
	SimpleMath::Vector2 bossBulletPosition[BOSS_SHOT_MAX];
	//�e�̔���
	bool bossBulletFlag[BOSS_SHOT_MAX];

	//�d���f�X�}�b�`
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