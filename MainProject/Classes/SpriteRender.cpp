#include"SpriteRender.h"
using namespace ID;
SpriteRender::SpriteRender() :spriteRECT(RectWH(0,0,0,0)),
	position(1000, DirectX::SimpleMath::Vector2(0, 0)),
	color(1000, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	angle(1000, 0.0f) {
}

void SpriteRender::Load(int ID){

	auto&& device = DXTK->Device;
	descHeap = DirectXTK::CreateDescriptorHeap(
		device, SpriteEnum::SpriteCount +
				PlayerEnum::PlayerCount +
				EnemyEnum::EnemyCount   +
				BossEnum::BossCount     +
				EffectEnum::EffectCount
	);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	switch (ID) {
	case PlayerEnum::RedPlayer:
		//プレイヤー画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_red.png", resourceUpload,
			descHeap, PlayerEnum::RedPlayer
		);
break;
	case PlayerEnum::BluePlayer:
		//プレイヤー画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_blue.png", resourceUpload,
			descHeap, PlayerEnum::BluePlayer
		);
break;
	case PlayerEnum::RedPlayerAttack:
		//攻撃画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Red_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::RedPlayerAttack
		);
break;
	case PlayerEnum::BluePlayerAttack:
		//攻撃画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Blue_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::BluePlayerAttack
		);
break;
	case  PlayerEnum::PlayerCollisionSprite:
		//当たり判定の位置、大きさ確認用(プレイヤー)
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"test.jpg", resourceUpload,
			descHeap, PlayerEnum::PlayerCollisionSprite
		);
break;
	case  PlayerEnum::PlayerBit:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Bit.png", resourceUpload,
			descHeap, PlayerEnum::PlayerBit
		);
break;
		//ビット攻撃
	//--------------------------------------------------------
	case PlayerEnum::PlayerBitAttack:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"WhiteBullet.png", resourceUpload,
			descHeap, PlayerEnum::PlayerBitAttack
		);
break;
		//--------------------------------------------------------
	case EffectEnum::RedPlayerAttackEffect:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"attack_effect_red.png", resourceUpload,
			descHeap, EffectEnum::RedPlayerAttackEffect
		);
break;
	case EffectEnum::BluePlayerAttackEffect:
			sprite = DirectXTK::CreateSpriteSRV(
				device, L"attack_effect_blue.png", resourceUpload,
				descHeap, EffectEnum::BluePlayerAttackEffect
			);
break;
	case BossEnum::Boss:

		//敵画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"enemy.png", resourceUpload,
			descHeap, BossEnum::Boss
		);
break;
	case BossEnum::BossBullet:
		//弾画像
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossBullet
		);
break;
	case BossEnum::BossInductionBullet:
		//誘導弾
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossInductionBullet
		);
break;
	case BossEnum::BossShotBullet:
		//誘導ビーム
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossShotBullet
		);
break;
	case SpriteEnum::BG:
		//背景
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"BackGroundForest.png", resourceUpload,
			descHeap, SpriteEnum::BG
		);
break;
	case BossEnum::BossHP:
		//敵HP
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"HP.png", resourceUpload,
			descHeap, BossEnum::BossHP
		);
break;
	case BossEnum::BossHPFrame:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"HP_haku.png", resourceUpload,
			descHeap, BossEnum::BossHPFrame
		);
break;
		//電流デスマッチ
	case BossEnum::BossBulletFrame:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossBulletFrame
		);
break;
	case EffectEnum::ExclamationMark:
		//危険信号(！マーク)
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"exclamation.png", resourceUpload,
			descHeap, EffectEnum::ExclamationMark
		);
break;
		//危険信号(トラテープ)
	case EffectEnum::ExclamationTape:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"exclamation_tape.png", resourceUpload,
			descHeap, EffectEnum::ExclamationTape
		);
break;
	case SpriteEnum::TV:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"TVoff.png", resourceUpload,
			descHeap, SpriteEnum::TV
		);
break;
	//case SpriteEnum::SpriteFont:
	//	sprite = DirectXTK::CreateSpriteFont(
	//		device, L"msgothic.spritefont", resourceUpload,
	//		descHeap, SpriteEnum::SpriteFont
	//	);
	//	return sprite;
		//敵、植物
	case EnemyEnum::Plant:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"EnemyPlant.png", resourceUpload,
			descHeap, EnemyEnum::Plant
		);
break;
	case EnemyEnum::PlantAttack:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Bullet.png", resourceUpload,
			descHeap, EnemyEnum::PlantAttack
		);
break;

	case EffectEnum::PlayerBitAttackEffect:
		sprite = DirectXTK::CreateSpriteSRV(
	device, L"BomEffect.png", resourceUpload,
	descHeap, EffectEnum::PlayerBitAttackEffect
);
 		   break;
//当たり判定の位置、大きさ確認用(斬撃)
	case SpriteEnum::Test:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"test.jpg", resourceUpload,
			descHeap, SpriteEnum::Test
		);
break;
	}

	RenderTargetState rtState(DXTK->BackBufferFormat, DXTK->DepthBufferFormat);
	SpriteBatchPipelineStateDescription pd(rtState, &CommonStates::NonPremultiplied);

	spriteBatch = DirectXTK::CreateSpriteBatch(device, resourceUpload, pd);
	auto&& viewport = DXTK->Screen.Viewport;
	spriteBatch->SetViewport(viewport);

	auto uploadResourcesFinished = resourceUpload.End(DXTK->CommandQueue);
	uploadResourcesFinished.wait();
}

void SpriteRender::Render(int count)
{
	//DXTK->ResetCommand();
	//DXTK->ClearRenderTarget(Colors::CornflowerBlue);
	ID3D12DescriptorHeap* heaps[] = { descHeap->Heap() };
	DXTK->CommandList->SetDescriptorHeaps((UINT)std::size(heaps), heaps);

	auto&& newSpriteBatch = spriteBatch.get();
	newSpriteBatch->Begin(DXTK->CommandList);


	newSpriteBatch->Draw(
		sprite.handle, sprite.size, position[count], //描画
		&spriteRECT,							     //画像のどこに表示するか
		color[count],                                //色の間引き方(カラーマスク)
		angle[count] * Mathf::Deg2Rad,               //回転の角度
		DirectX::SimpleMath::Vector2(sprite.size.x / 2, sprite.size.y / 2),//軸
		1.0f										  //画像サイズ(Spriteのサイズではない)
	);

	spriteBatch->End();
	//DXTK->ExecuteCommandList();
}
