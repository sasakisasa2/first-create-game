//
// MainScene.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"
#include "DontDestroyOnLoad.h"
#include <random>

using namespace SimpleMath;
// Initialize member variables.
MainScene::MainScene()
{

}

// Start is called after the scene is created.
void MainScene::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void MainScene::LoadAssets()
{
	auto&& device = DXTK->Device;
	descHeap = DirectXTK::CreateDescriptorHeap(
		device, SpriteCount +
				PlayerCount +
				EnemyCount +
				BossCount +
				EffectCount
	);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

#pragma region プレイヤー画像
	DirectXTK::Sprite playerSprite;
	if (DontDestroy->attackAttribute == 2) {
		playerSprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_red2.png", resourceUpload,
			descHeap, PlayerEnum::Player
		);
	}
	if (DontDestroy->attackAttribute == 1) {
		playerSprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_blue.png", resourceUpload,
			descHeap, PlayerEnum::Player
		);	
	}
#pragma endregion
#pragma region プレイヤー攻撃画像
	DirectXTK::Sprite playerAttackSprite;
	if (DontDestroy->attackAttribute == 2) {
		//攻撃画像
		playerAttackSprite = DirectXTK::CreateSpriteSRV(
			device, L"Red_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::PlayerAttack
		);
	}
	if (DontDestroy->attackAttribute == 1) {
		//攻撃画像
		playerAttackSprite = DirectXTK::CreateSpriteSRV(
			device, L"Blue_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::PlayerAttack
		);
	}
#pragma endregion
#pragma region プレイヤー当たり判定画像
	DirectXTK::Sprite playerCollisionSprite;
	//当たり判定の位置、大きさ確認用(プレイヤー)
	playerCollisionSprite = DirectXTK::CreateSpriteSRV(
		device, L"test.jpg", resourceUpload,
		descHeap, PlayerEnum::PlayerCollisionSprite
	);
#pragma endregion
#pragma region プレイヤー攻撃エフェクト
	//if (DontDestroy->attackColor==2) {
	//	playerAttackEffectSprite = DirectXTK::CreateSpriteSRV(
	//		device, L"attack_effect_red.png", resourceUpload,
	//		descHeap, PlayerEnum::PlayerAttackEffect
	//	);
	//}

	//if (DontDestroy->attackColor==1) {
	//	playerAttackEffectSprite = DirectXTK::CreateSpriteSRV(
	//		device, L"attack_effect_blue.png", resourceUpload,
	//		descHeap, PlayerEnum::PlayerAttackEffect
	//	);
	//}
#pragma endregion

	classPlayer.LoadAssets(playerSprite, playerAttackSprite, playerCollisionSprite);

#pragma region 攻撃ビット画像

	DirectXTK::Sprite bitSprite;
	bitSprite = DirectXTK::CreateSpriteSRV(
		device, L"Bit.png", resourceUpload,
		descHeap, PlayerEnum::PlayerBit
	);

#pragma endregion
#pragma region ビットの攻撃画像

	DirectXTK::Sprite bitAttackSprite;
	bitAttackSprite = DirectXTK::CreateSpriteSRV(
		device, L"WhiteBullet.png", resourceUpload,
		descHeap, PlayerEnum::PlayerBitAttack
	);
#pragma endregion


	for (int bitcount = 0; bitcount < 10; bitcount++) {
		playerBit[bitcount].Load(bitSprite, bitAttackSprite);
	}
#pragma region PlantEnemySprite

	DirectXTK::Sprite plantSprite;
	plantSprite = DirectXTK::CreateSpriteSRV(
		device, L"EnemyPlant.png", resourceUpload,
		descHeap, EnemyEnum::Enemy
	);
#pragma endregion

#pragma region PlantAttackSprite

	DirectXTK::Sprite plantAttackSprite;
	plantAttackSprite = DirectXTK::CreateSpriteSRV(
		device, L"Bullet.png", resourceUpload,
		descHeap, EnemyEnum::EnemyAttack
	);
#pragma endregion
	for (int i = 0; i < PLANT_MAX; i++) {
		enemyPlant[i].Load(plantSprite, plantAttackSprite);
	}

	DirectXTK::Sprite sprite[4];
	//敵画像
	sprite[0] = DirectXTK::CreateSpriteSRV(
		device, L"enemy.png", resourceUpload,
		descHeap, BossEnum::Boss
	);

	//弾画像
	sprite[1] = DirectXTK::CreateSpriteSRV(
		device, L"bullet.png", resourceUpload,
		descHeap, BossEnum::BossBullet
	);
	//敵HP
	sprite[2] = DirectXTK::CreateSpriteSRV(
		device, L"HP.png", resourceUpload,
		descHeap, BossEnum::BossHP
	);
	sprite[3] = DirectXTK::CreateSpriteSRV(
		device, L"HP_haku.png", resourceUpload,
		descHeap, BossEnum::BossHPFrame
	);

	////誘導弾
	//bossInductionBulletSprite = DirectXTK::CreateSpriteSRV(
	//	device, L"bullet.png", resourceUpload,
	//	descHeap, BossEnum::BossInductionBullet
	//);

	////誘導ビーム
	//bossBulletShotSprite = DirectXTK::CreateSpriteSRV(
	//	device, L"bullet.png", resourceUpload,
	//	descHeap, BossEnum::BossShotBullet
	//);

	boss.Load(sprite[0], sprite[1], sprite[2], sprite[3]);

	//背景
	backGroundSprite = DirectXTK::CreateSpriteSRV(
		device, L"BackGroundForest.png", resourceUpload,
		descHeap, SpriteEnum::BG
	);

	//電流デスマッチ
	bossFrameSprite = DirectXTK::CreateSpriteSRV(
		device, L"bullet.png", resourceUpload,
		descHeap, BossEnum::BossBulletFrame
	);
	//危険信号(！マーク)
	bossExclamationSprite = DirectXTK::CreateSpriteSRV(
		device, L"exclamation.png", resourceUpload,
		descHeap, EffectEnum::ExclamationMark
	);
	//危険信号(トラテープ)
	bossExclamationTapeSprite = DirectXTK::CreateSpriteSRV(
		device, L"exclamation_tape.png", resourceUpload,
		descHeap, EffectEnum::ExclamationTape
	);

	offOnSprite = DirectXTK::CreateSpriteSRV(
		device, L"TVoff.png", resourceUpload,
		descHeap, SpriteEnum::TV
	);

	spriteFont = DirectXTK::CreateSpriteFont(
		device, L"msgothic.spritefont", resourceUpload,
		descHeap, SpriteEnum::SpriteFont
	);
	//敵、植物
	//-------------------------------------------------------
	//-------------------------------------------------------

	//bitAttackEffectSprite = DirectXTK::CreateSpriteSRV(
	//	device, L"BomEffect.png", resourceUpload,
	//	descHeap, EffectEnum::PlayerBitAttackEffect
	//);
	//当たり判定の位置、大きさ確認用(斬撃)
	test_sprite[1] = DirectXTK::CreateSpriteSRV(
		device, L"test.jpg", resourceUpload,
		descHeap, SpriteEnum::Test
	);



	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);
	SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);

	sprite_batch_ = DirectXTK::CreateSpriteBatch(device, resourceUpload, pd);
	auto&& viewport = DXTK->Screen.Viewport;
	sprite_batch_->SetViewport(viewport);

	auto uploadResourcesFinished = resourceUpload.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();
}

// Initialize a variable and audio resources.
void MainScene::Initialize()
{
	std::random_device rd;
	random = std::mt19937(rd());

	

	//プレイヤー
	//---------------------------------------------------
	classPlayer.Initialize();
	for (int bitcount = 0; bitcount < BIT_MAX; bitcount++) {
		if (bitcount % 2 == 0) {
			playerBit[bitcount].Initialize(360.0f / (float)BIT_MAX * ((float)bitcount + 1.0f), 
									       false, //回転方向
										   false );//湾曲方向
			continue;
		}
		    playerBit[bitcount].Initialize(360.0f / (float)BIT_MAX * ((float)bitcount + 1.0f),
				                           false , //回転方向
				                           false);//湾曲方向
		    continue;
	}
	for (int i = 0; i < PLANT_MAX; i++) {
		enemyPlant[i].Initialize();
	}
	isEnemyAttack = false;

	boss.Initialize();

#pragma region BitAttackEffect

	//for (int i = 0; i < BIT_ATTACK_NUMBER; i++) {
	//	bitAttackCollisionDetection[i] = true;
	//	bitAttackCoolTime[i] = 0;
	//	//攻撃ビットエフェクト
	//	bitAttackEffectDrowFlag[i] = false;
	//	bitAttackEffectTime[i] = 0;
	//	bitAttackEffectTransparency[i] = 0;
	//}
#pragma endregion

#pragma region PlayerAttackEffect
	//for (int i = 0; i < PLAYER_ATTACK_MAX; i++) {
	//	playerAttackEffectDrowFlag[i] = false;
	//	playerAttackEffectTime[i] = 0;
	//}

	//playerAttackEffectSprite.size.x = playerAttackEffectSize;
	//playerAttackEffectSprite.size.y = playerAttackEffectSize;
#pragma endregion
	test_sprite[1].size.x = 30;
	test_sprite[1].size.y = 30;

	offOnFlag = true;

	//背景位置
	backGroundPosition = Vector2(0, 0);

	//危険信号
	bossExclamationSprite.size.x = frame_size / 2;
	bossExclamationSprite.size.y = frame_size / 2;

	bossExclamationTapeSprite.size.x = 1280;
	bossExclamationTapeSprite.size.y = frame_size / 4;


	//HP
	bossHpSprite[0].size.x = 1100;
	bossHpSprite[0].size.y = 50;
	bossHpSprite[1].size.x = 1120;
	bossHpSprite[1].size.y = 60;

	hit_se_ = DirectXTK::CreateSound(DXTK->Audio.Engine, L"bullet_hit_se.wav");
	hit_se_instance = hit_se_->CreateInstance();
}

// Releasing resources required for termination.
void MainScene::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void MainScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void MainScene::OnRestartSound()
{

}

// Updates the scene.
NextScene MainScene::Update(const float deltaTime)
{
	backGroundPosition.x -= 500.0f * deltaTime;
	if (backGroundPosition.x <= -3840 + 1280) {
		backGroundPosition.x += 3840 - 1280;
	}
	//プレイヤー
	//----------------------------------------------------------------------------
	classPlayer.Update();

	for (int bitcount = 0; bitcount < BIT_MAX; bitcount++) {
		playerBit[bitcount].Update(classPlayer.GetPlayerPosition());
	}

	if (InputSystem.Keyboard.wasPressedThisFrame.J) {
		isEnemyAttack = true;
	}

	if (isEnemyAttack) {
		for (int i = 0; i < PLANT_MAX; i++) {
			enemyPlant[i].Update(classPlayer.GetPlayerPosition());
		}
	}
	//ビット移動
	//------------------------------------------------------------------------------
	delTime = deltaTime;

	if (InputSystem.Keyboard.wasPressedThisFrame.B) {
		bossFlag = true;
	}
	boss.Update(classPlayer.GetPlayerPosition());
	//boss
	//------------------------------------------------------------------------------------
			//ボス登場から攻撃するまで
	if (bossFlag) {
		//bossAttackStartTime += deltaTime;
		//if (bossAttackStartTime >= 1 && bossAttackStartTime <= 1.25f) {
		//	bossBulletPlaceFlag[0] = true;
		//}


		//if (bossBulletShotPosition[BOSS_BULLET_QUANTITY - 1].y >= 800) {
		//	for (int i = 0; i < BOSS_QUANTITY_ATTACK; i++) {
		//		bossExclamationFlag = true;
		//		bossBulletShotFlag[i] = false;
		//		bossBulletPlaceFlag[i] = false;
		//		bossBulletShotTime[i] = 0;
		//	}
		//}

		//危険信号
		if (bossExclamationFlag) {
			bossExclamationTime += deltaTime;
			if (!bossExclamationSignalFlag) {
				bossExclamationSignalTime += bossExclamationSignalInterval * deltaTime;
				if (bossExclamationSignalTime >= 1) {
					bossExclamationSignalFlag = true;
				}
			}
			else {
				bossExclamationSignalTime -= bossExclamationSignalInterval * deltaTime;
				if (bossExclamationSignalTime <= 0) {
					bossExclamationSignalFlag = false;
				}
			}
			if (bossExclamationTime >= END_SIGNAL_TIME) {
				bossExclamationFlag = false;
				bossExclamationSignalTime = 0;
				bossExclamationFlag = 0;
				bossExclamationSignalFlag = false;
				bossAttackFramePlaceFlag = true;
				bossShotFlag = true;
			}
		}
	}
	//-----------------------------------------------------------------------------------------
	//MainScene::CollisionSummarize();
	

	return NextScene::Continue;
}

// Draws the scene.
void MainScene::Render()
{
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);
	ID3D12DescriptorHeap* heaps[] = { descHeap->Heap() };
	DXTK->CommandList->SetDescriptorHeaps((UINT)std::size(heaps), heaps);

	auto&& spriteBatch = sprite_batch_.get();
	spriteBatch->Begin(DXTK->CommandList);

	spriteBatch->Draw(
		backGroundSprite.handle, backGroundSprite.size, backGroundPosition,
		nullptr,//画像のどこに表示するか
		Colors::White,//色の間引き方(カラーマスク)
		0.0f * Mathf::Deg2Rad,
		Vector2(0, 0),//回転の角度と軸
		1.0f
	);
	classPlayer.Render(spriteBatch);
	for (int bitcount = 0; bitcount < 10; bitcount++) {
		playerBit[bitcount].Render(spriteBatch);
	}
	for (int i = 0; i < PLANT_MAX; i++) {
		enemyPlant[i].Render(spriteBatch);
	}
	boss.Render(spriteBatch);
	if (bossFlag) {
		RECT gauge_src = RectWH(0, 0, 1085 * ((float)bossHpLow / BOSS_HP), 50);
		spriteBatch->Draw(
			bossHpSprite[0].handle, bossHpSprite[0].size, Vector2(60, 650),
			&gauge_src
		);
		spriteBatch->Draw(
			bossHpSprite[1].handle, bossHpSprite[1].size, Vector2(50, 645)
		);
	}

	//攻撃エフェクト 
	//-----------------------------------------------------------------------------
	//for (int i = 0; i < PLAYER_ATTACK_MAX; i++) {
	//	if (playerAttackEffectDrowFlag[i]) {
	//		spriteBatch->Draw(
	//			playerAttackEffectSprite.handle, playerAttackEffectSprite.size,
	//			playerAttackEffectPosition[i],
	//			nullptr,//画像のどこに表示するか
	//			Colors::White,
	//			0.0f * Mathf::Deg2Rad, Vector2(playerAttackEffectSize / 2, playerAttackEffectSize / 2),//回転の角度と軸
	//			1.0f
	//		);
	//	}
	//}
	//ビット攻撃エフェクト
	//for (int i = 0; i < BIT_ATTACK_NUMBER; i++) {
	//	if (bitAttackEffectDrowFlag[i]) {
	//		spriteBatch->Draw(
	//			bitAttackEffectSprite.handle, bitAttackEffectSprite.size,
	//			bitAttackEffectPosition[i],
	//			nullptr,//画像のどこに表示するか
	//			Color(1.0f, 1.0f, 1.0f, bitAttackEffectTransparency[i]),
	//			0.0f * Mathf::Deg2Rad, Vector2(bitAttackEffectSize / 2, bitAttackEffectSize / 2),//回転の角度と軸
	//			1.0f
	//		);
	//	}
	//}
	//----------------------------------------------------------------------------

	//危険信号
	if (bossExclamationFlag) {
		//上
		//！マーク
		spriteBatch->Draw(
			bossExclamationSprite.handle, bossExclamationSprite.size,
			Vector2(640 - (frame_size / 4), (frame_size / 4)),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		//トラテープ
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(0, 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(0, (frame_size / 4 * 3)),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		//下
		spriteBatch->Draw(
			bossExclamationSprite.handle, bossExclamationSprite.size,
			Vector2(640 - (frame_size / 4), 720 - (frame_size / 4 * 3)),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		//トラテープ
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(0, 720 - (frame_size / 4) + 5),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(0, 720 - (frame_size)),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		//左
		spriteBatch->Draw(
			bossExclamationSprite.handle, bossExclamationSprite.size,
			Vector2((frame_size / 4), 360 - (frame_size / 4)),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(0, 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			90.0f * Mathf::Deg2Rad, Vector2(0, (frame_size / 4)),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2((frame_size / 4 * 3), 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			90.0f * Mathf::Deg2Rad, Vector2(0, (frame_size / 4)),//回転の角度と軸
			1.0f
		);
		//右
		spriteBatch->Draw(
			bossExclamationSprite.handle, bossExclamationSprite.size,
			Vector2(1280 - (frame_size / 4 * 3), 360 - (frame_size / 4)),
			nullptr,//画像のどこに表示するか 
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(1280, 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			90.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
		spriteBatch->Draw(
			bossExclamationTapeSprite.handle, bossExclamationTapeSprite.size,
			Vector2(1280 - (frame_size / 4 * 3), 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, bossExclamationSignalTime),//色の間引き方(カラーマスク)
			90.0f * Mathf::Deg2Rad, Vector2(0, 0),//回転の角度と軸
			1.0f
		);
	}

	//spriteBatch->Draw(
	//	test_sprite[1].handle, test_sprite[1].size,
	//	attack_bit_guide,
	//	nullptr,//画像のどこに表示するか
	//	Colors::Black,//色の間引き方(カラーマスク)
	//	0.0f * Mathf::Deg2Rad, Vector2(15, 15),//回転の角度と軸
	//	1.0f
	//);



	if (onFlag) {
		if (offOnFlag) {
			spriteBatch->Draw(
				offOnSprite.handle, offOnSprite.size,
				Vector2(0, 0)
			);
		}
	}

	//std::wstring time_string = std::to_wstring(bitTheta[0]);

	//// 小数3桁目以降非表示
	//int period = time_string.find_first_of('.', 0);
	//time_string[period + 0] = '\0';

	//Vector2 time_origin = spritefont->MeasureString(time_string.c_str());
	//spritefont->DrawString(
	//	spriteBatch,
	//	time_string.c_str(),
	//	Vector2(700.0f, 44.0f),
	//	Color(255, 255, 255),
	//	0.0f, time_origin);



	spriteBatch->End();
	DXTK->ExecuteCommandList();
}

//-----------------プレイヤー関係関数---------------------
///// <summary>
///// プレイヤー攻撃のエフェクトを出す関数
///// </summary>
//void MainScene::PlayerAttackEffectDrow() {
//	for (int i = 0; i < PLAYER_ATTACK_MAX; i++) {
//		if (playerAttackEffectDrowFlag[i]) {
//			playerAttackEffectTime[i] += delTime;
//			if (playerAttackEffectTime[i] >= playerAttackEffectDrowTime) {
//				playerAttackEffectTime[i] = 0;
//				playerAttackEffectDrowFlag[i] = false;
//			}
//		}
//	}
//}
///// <summary>
///// ビット攻撃の当たり判定のオフ時間を計算し、オンにする関数
///// </summary>
//void MainScene::BitAttackCollisionCoolTime()
//{
//	for (int i = 0; i < BIT_ATTACK_NUMBER; i++) {
//		if (!bitAttackCollisionDetection[i]) {
//			bitAttackCoolTime[i] += delTime;
//			if (bitAttackCoolTime[i] >= bitAttackCoolTimeEnd) {
//				bitAttackCollisionDetection[i] = true;
//				bitAttackCoolTime[i] = 0;
//			}
//		}
//	}
//}
///// <summary>
///// ビットの攻撃のエフェクトを出す関数
///// </summary>
//void MainScene::BitAttackEffectDrow() {
//	for (int i = 0; i < BIT_ATTACK_NUMBER; i++) {
//		if (bitAttackEffectDrowFlag[i]) {
//			bitAttackEffectTime[i] += delTime;
//			if (bitAttackEffectTime[i] >= bitAttackEffectDrowTime) {
//				bitAttackEffectTransparency[i] -= (1 / bitAttackEffectTransparencyTime) * delTime;
//				if (bitAttackEffectTransparency[i] <= 0) {
//					bitAttackEffectDrowFlag[i] = false;
//					bitAttackEffectTime[i] = 0;
//				}
//			}
//		}
//	}
//}
//--------------------------------------------------------
//--------------------------------------------------------

//--------------------当たり判定関係----------------------
/// <summary>
/// 当たり判定を纏めて指示する関数
/// </summary>
//void MainScene::CollisionSummarize()
//{
//	MainScene::PlayerCollision();
//
//	MainScene::PlayerAttackCollision();
//
//	MainScene::PlayerBitAttackCollision();
//}
//
////----------------プレイヤーの当たり判定------------------
///// <summary>
///// プレイヤー関連の当たり判定をしている関数
///// </summary>
//void MainScene::PlayerCollision()
//{
//	//プレイヤー当たり判定
//	Rectangle player_collision;
//	player_collision.x = classPlayer.GetPlayerPosition().x - (100 / 6);
//	player_collision.y = classPlayer.GetPlayerPosition().y - (100 / 6);
//	player_collision.width = 20;
//	player_collision.height = 20;
//
//	MainScene::PlayerBossFrameDetection(player_collision);
//	MainScene::PlayerBossQuantityDetection(player_collision);
//	MainScene::PlayerBossInductionDetection(player_collision);
//	MainScene;; PlayerBossShotDetection(player_collision);
//}
///// <summary>
///// プレイヤーとボスフレーム攻撃の当たり判定関数
///// </summary>
///// <param name="プレイヤーのcollision"></param>
//void MainScene::PlayerBossFrameDetection(Rectangle collision) {
//	//電流デスマッチ当たり判定
//	Rectangle frame_collision[FRAME_BULLET_MAX];
//	for (int i = 0; i < FRAME_BULLET_MAX; i++) {
//		frame_collision[i].x = frame_pos[i].x;
//		frame_collision[i].y = frame_pos[i].y;
//		frame_collision[i].width = 90;
//		frame_collision[i].height = 90;
//	}
//	//プレイヤーと電流デスマッチ
//	for (int i = 0; i < FRAME_BULLET_MAX; i++) {
//		if (collision.Intersects(frame_collision[i])) {
//			hit_se_instance->Play();
//		}
//	}
//}
///// <summary>
/////  プレイヤーとボス誘導弾攻撃の当たり判定関数
///// </summary>
///// <param name="collision"></param>
//void MainScene::PlayerBossQuantityDetection(Rectangle collision) {
//	//時期狙い弾
//	Rectangle quantity_collision[AIMSHOT_BULLET_MAX];
//	for (int i = 0; i < AIMSHOT_BULLET_MAX; i++) {
//		quantity_collision[i].x = bossBulletShotPosition[i].x + 5;
//		quantity_collision[i].y = bossBulletShotPosition[i].y + 5;
//		quantity_collision[i].width = 40;
//		quantity_collision[i].height = 40;
//	}
//	//プレイヤーと時期狙い弾
//	for (int i = 0; i < AIMSHOT_BULLET_MAX; i++) {
//		if (collision.Intersects(quantity_collision[i])) {
//			hit_se_instance->Play();
//		}
//	}
//}
///// <summary>
/////  プレイヤーとボス自機狙い弾攻撃の当たり判定関数
///// </summary>
///// <param name="collision"></param>
//void MainScene::PlayerBossInductionDetection(Rectangle collision) {
//	//誘導弾
//	Rectangle induction_collision;
//	induction_collision.x = bossInductionBulletPosition.x - 25 * bossInductionBulletVolume + 10;
//	induction_collision.y = bossInductionBulletPosition.y - 25 * bossInductionBulletVolume + 10;
//	induction_collision.width = 50 * bossInductionBulletVolume - 10;
//	induction_collision.height = 50 * bossInductionBulletVolume - 10;
//	//プレイヤーと誘導弾
//	if (collision.Intersects(induction_collision)) {
//		hit_se_instance->Play();
//	}
//}
///// <summary>
/////  プレイヤーとボス通常弾攻撃の当たり判定関数
///// </summary>
///// <param name="collision"></param>
//void MainScene::PlayerBossShotDetection(Rectangle collision) {
//	//ばらまき弾
//	Rectangle shot_collision[NORMAL_SHOT_BULLET_MAX];
//	for (int i = 0; i < NORMAL_SHOT_BULLET_MAX; i++) {
//		shot_collision[i].x = bossBulletPosition[i].x + 5;
//		shot_collision[i].y = bossBulletPosition[i].y + 5;
//		shot_collision[i].width = 20;
//		shot_collision[i].height = 20;
//	}
//	//プレイヤーとばらまき弾
//	for (int i = 0; i < NORMAL_SHOT_BULLET_MAX; i++) {
//		if (collision.Intersects(shot_collision[i])) {
//			hit_se_instance->Play();
//		}
//	}
//}
////--------------プレイヤー攻撃の当たり判定----------------
///// <summary>
///// プレイヤーの当たり判定をしている関数
///// </summary>
//void MainScene::PlayerAttackCollision() {
//	//攻撃当たり判定
//	Rectangle playerAttackCollision[PLAYER_ATTACK_MAX];
//	for (int i = 0; i < PLAYER_ATTACK_MAX; i++) {
//		playerAttackCollision[i].x = attackPosition[i].x - attackBulletSizeX / 2;
//		playerAttackCollision[i].y = attackPosition[i].y - attackBulletSizeX / 2;
//		playerAttackCollision[i].width = attackBulletSizeX;
//		playerAttackCollision[i].height = attackBulletSizeX;
//		MainScene::PlayerAttackBossDetection(playerAttackCollision[i], i);
//		MainScene::PlayerAttackPlantDetection(playerAttackCollision[i], i);
//	}
//}
///// <summary>
/////  プレイヤー攻撃とボスの当たり判定関数
///// </summary>
///// <param name="collision"></param>
///// <param name="number"></param>
//void MainScene::PlayerAttackBossDetection(Rectangle collision, int number) {
//	//敵当たり判定
//	Rectangle boss_collision;
//	boss_collision.x = bossPosition.x - 75;
//	boss_collision.y = bossPosition.y - 75;
//	boss_collision.width = 150;
//	boss_collision.height = 150;
//
//	if (attackFlag[number]) {
//		if (collision.Intersects(boss_collision)) {
//			bossHpLow -= attackDamage;
//			attackFlag[number] = false;
//			playerAttackEffectPosition[number] = attackPosition[number];
//			playerAttackEffectDrowFlag[number] = true;
//		}
//	}
//}
///// <summary>
/////  プレイヤー攻撃と雑魚敵(植物）の当たり判定関数
///// </summary>
///// <param name="collision"></param>
//void MainScene::PlayerAttackPlantDetection(Rectangle collision, int number)
//{
//	float plantSizeThirdX = plantSizeX / 3;
//	float plantSizeHalfY = plantSizeY / 2;
//	//植物エネミーの当たり判定
//	if (plantAdventFlag) {
//		Rectangle enemyPlantCollision;
//		enemyPlantCollision.x = plantPosition.x - plantSizeThirdX / 2;
//		enemyPlantCollision.y = plantPosition.y - plantSizeHalfY / 2 + 10;
//		enemyPlantCollision.width = plantSizeThirdX;
//		enemyPlantCollision.height = plantSizeHalfY;
//		if (attackFlag[number]) {
//			if (collision.Intersects(enemyPlantCollision)) {
//				plantHp -= attackDamage;
//				attackFlag[number] = false;
//				playerAttackEffectPosition[number] = attackPosition[number];
//				playerAttackEffectDrowFlag[number] = true;
//			}
//		}
//	}
//}
////------------------------------------------------------------------------------------
//
////-----------プレイヤービット攻撃の当たり判定-------------
///// <summary>
///// ビット攻撃関係の当たり判定をしている関数
///// </summary>
//void MainScene::PlayerBitAttackCollision() {
//	//ビット攻撃の当たり判定
//	Rectangle bitAttackCollision[BIT_ATTACK_NUMBER];
//	for (int i = 0; i < BIT_ATTACK_NUMBER; i++) {
//		bitAttackCollision[i].x = bitAttackPosition[i].x - bitSize / 2;
//		bitAttackCollision[i].y = bitAttackPosition[i].y - bitSize / 2;
//		bitAttackCollision[i].width = bitSize;
//		bitAttackCollision[i].height = bitSize;
//		MainScene::PlayerBitAttackBossDetection(bitAttackCollision[i], i);
//		MainScene::PlayerBitAttackPlantDetection(bitAttackCollision[i], i);
//	}
//}
///// <summary>
///// ビット攻撃と植物雑魚敵の当たり判定をする関数
///// </summary>
///// <param name="collision"></param>
///// <param name="number"></param>
//void MainScene::PlayerBitAttackPlantDetection(Rectangle collision, int number) {
//	float plantSizeThirdX = plantSizeX / 3;
//	float plantSizeHalfY = plantSizeY / 2;
//	//植物エネミーの当たり判定
//	if (plantAdventFlag) {
//		Rectangle enemyPlantCollision;
//		enemyPlantCollision.x = plantPosition.x - plantSizeThirdX / 2;
//		enemyPlantCollision.y = plantPosition.y - plantSizeHalfY / 2 + 10;
//		enemyPlantCollision.width = plantSizeThirdX;
//		enemyPlantCollision.height = plantSizeHalfY;
//		if (bitAttackCollisionDetection[number]) {
//			if (collision.Intersects(enemyPlantCollision)) {
//				plantHp -= bitAttackDamage;
//				bitAttackCollisionDetection[number] = false;
//				bitAttackEffectPosition[number] = bitAttackPosition[number];
//				bitAttackEffectTransparency[number] = 1;
//				bitAttackEffectDrowFlag[number] = true;
//			}
//		}
//	}
//}
///// <summary>
///// プレイヤービット攻撃とボスの当たり判定
///// </summary>
///// <param name="collision"></param>
///// <param name="number"></param>
//void MainScene::PlayerBitAttackBossDetection(Rectangle collision, int number) {
//	//敵当たり判定
//	Rectangle boss_collision;
//	boss_collision.x = bossPosition.x - 75;
//	boss_collision.y = bossPosition.y - 75;
//	boss_collision.width = 150;
//	boss_collision.height = 150;
//
//	//ビット攻撃の判定
//	if (bitAttackCollisionDetection[number]) {
//		if (collision.Intersects(boss_collision)) {
//			bossHpLow -= bitAttackDamage;
//			bitAttackCollisionDetection[number] = false;
//			bitAttackEffectPosition[number] = bitAttackPosition[number];
//			bitAttackEffectTransparency[number] = 1;
//			bitAttackEffectDrowFlag[number] = true;
//		}
//	}
//}
////---------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------

