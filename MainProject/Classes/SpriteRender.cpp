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
		//�v���C���[�摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_red.png", resourceUpload,
			descHeap, PlayerEnum::RedPlayer
		);
break;
	case PlayerEnum::BluePlayer:
		//�v���C���[�摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Player_blue.png", resourceUpload,
			descHeap, PlayerEnum::BluePlayer
		);
break;
	case PlayerEnum::RedPlayerAttack:
		//�U���摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Red_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::RedPlayerAttack
		);
break;
	case PlayerEnum::BluePlayerAttack:
		//�U���摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"Blue_Bullet.png", resourceUpload,
			descHeap, PlayerEnum::BluePlayerAttack
		);
break;
	case  PlayerEnum::PlayerCollisionSprite:
		//�����蔻��̈ʒu�A�傫���m�F�p(�v���C���[)
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
		//�r�b�g�U��
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

		//�G�摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"enemy.png", resourceUpload,
			descHeap, BossEnum::Boss
		);
break;
	case BossEnum::BossBullet:
		//�e�摜
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossBullet
		);
break;
	case BossEnum::BossInductionBullet:
		//�U���e
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossInductionBullet
		);
break;
	case BossEnum::BossShotBullet:
		//�U���r�[��
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossShotBullet
		);
break;
	case SpriteEnum::BG:
		//�w�i
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"BackGroundForest.png", resourceUpload,
			descHeap, SpriteEnum::BG
		);
break;
	case BossEnum::BossHP:
		//�GHP
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
		//�d���f�X�}�b�`
	case BossEnum::BossBulletFrame:
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"bullet.png", resourceUpload,
			descHeap, BossEnum::BossBulletFrame
		);
break;
	case EffectEnum::ExclamationMark:
		//�댯�M��(�I�}�[�N)
		sprite = DirectXTK::CreateSpriteSRV(
			device, L"exclamation.png", resourceUpload,
			descHeap, EffectEnum::ExclamationMark
		);
break;
		//�댯�M��(�g���e�[�v)
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
		//�G�A�A��
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
//�����蔻��̈ʒu�A�傫���m�F�p(�a��)
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
		sprite.handle, sprite.size, position[count], //�`��
		&spriteRECT,							     //�摜�̂ǂ��ɕ\�����邩
		color[count],                                //�F�̊Ԉ�����(�J���[�}�X�N)
		angle[count] * Mathf::Deg2Rad,               //��]�̊p�x
		DirectX::SimpleMath::Vector2(sprite.size.x / 2, sprite.size.y / 2),//��
		1.0f										  //�摜�T�C�Y(Sprite�̃T�C�Y�ł͂Ȃ�)
	);

	spriteBatch->End();
	//DXTK->ExecuteCommandList();
}
