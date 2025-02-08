//
// titl.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"
#include "DontDestroyOnLoad.h"

using namespace SimpleMath;

// Initialize member variables.
titl::titl()
{

}

// Start is called after the scene is created.
void titl::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void titl::LoadAssets()
{
	auto&& device = DXTK->Device;
	des = DirectXTK::CreateDescriptorHeap(device, 200);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	//テレビ
	TV_sp = DirectXTK::CreateSpriteSRV(
		device, L"TV.png", resourceUpload,
		des, 0
	);

	error_sp = DirectXTK::CreateSpriteSRV(
		device, L"error.png", resourceUpload,
		des, 1
	);

	off_on_sp[0] = DirectXTK::CreateSpriteSRV(
		device, L"TVoff.png", resourceUpload,
		des, 2
	);

	background_sp = DirectXTK::CreateSpriteSRV(
		device, L"background.png", resourceUpload,
		des, 22
	);

	Game_background_sp = DirectXTK::CreateSpriteSRV(
		device, L"background.png", resourceUpload,
		des, 23
	);

	background_frame_sp = DirectXTK::CreateSpriteSRV(
		device, L"frames.png", resourceUpload,
		des, 24
	);

	enter_sp = DirectXTK::CreateSpriteSRV(
		device, L"enter.png", resourceUpload,
		des, 25
	);

	boss_lush_sp = DirectXTK::CreateSpriteSRV(
		device, L"Boss_Lush.png", resourceUpload,
		des, 26
	);

	red_sp = DirectXTK::CreateSpriteSRV(
		device, L"HP.png", resourceUpload,
		des, 27
	);

	blue_sp = DirectXTK::CreateSpriteSRV(
		device, L"blue.png", resourceUpload,
		des, 28
	);

	VS_sp = DirectXTK::CreateSpriteSRV(
		device, L"VS.png", resourceUpload,
		des, 29
	);

	player_sp = DirectXTK::CreateSpriteSRV(
		device, L"Player.png", resourceUpload,
		des, 30
	);

	enemy_sp = DirectXTK::CreateSpriteSRV(
		device, L"enemy.png", resourceUpload,
		des, 31
	);

	select_sp = DirectXTK::CreateSpriteSRV(
		device, L"Character Selection.png", resourceUpload,
		des, 32
	);
	
	select_arrow_sp = DirectXTK::CreateSpriteSRV(
		device, L"Select.png", resourceUpload,
		des, 33
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
void titl::Initialize()
{
	for (int i = 0; i < off_on_MAX; i++) {
		off_on_flag[i] = false;
	}

	boss_lush_sp.size.x = 800;
	boss_lush_sp.size.y = 620;

	off_on_sp[0].size.x = 1280;
	off_on_sp[0].size.y = 720;

	error_sp.size.x = 1280;
	error_sp.size.y = 720 * 3;

	background_sp.size.x = 1280;
	background_sp.size.y = 720;

	Game_background_sp.size.x = 1280;
	Game_background_sp.size.y = 720;

	red_sp.size.x = 1280;
	red_sp.size.y = 720;

	red_pos = Vector2(-1280, 0);



	blue_sp.size.x = 1280;
	blue_sp.size.y = 720;

	blue_pos = Vector2(1280, 0);

	VS_sp.size.x = 1280;
	VS_sp.size.y = 720;

	VS_pos = Vector2(0, -720);

	player_sp.size.x = 300;
	player_sp.size.y = 300;

	player_pos = Vector2(-470, 310);

	enemy_sp.size.x = 300;
	enemy_sp.size.y = 300;

	enemy_pos = Vector2(1500, 310);

	select_sp.size.x = 1000;
	select_sp.size.y = 300;

	select_pos = Vector2(100, 0);

	select_arrow_sp.size.x = 100;
	select_arrow_sp.size.y = 100;
}

// Releasing resources required for termination.
void titl::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void titl::OnDeviceLost()
{

}

// Restart any looped sounds here
void titl::OnRestartSound()
{

}

// Updates the scene.
NextScene titl::Update(const float deltaTime)
{

	if (InputSystem.Keyboard.isPressed.Enter && !on_flag) {
		TV_expansion_flag = true;
		signal = 0;
	}
	if (!on_flag && !TV_expansion_flag) {
		if (!signal_flag) {
			signal += deltaTime;
			if (signal >= 1) {
				signal_flag = true;
			}
		}
		else {
			signal -= deltaTime;
			if (signal <= 0) {
				signal_flag = false;
			}
		}
	}



	TV_sp.size.x = TV_size_x;
	TV_sp.size.y = TV_size_y;
	error_pos.y += 2000.0f * deltaTime;
	if (error_pos.y <= -720.0f * 3.0f + 720.0f) {
		error_pos.y += 720.0f * 3.0f - 720.0f;
	}
	if (error_pos.y >= 0.0f) {
		error_pos.y += -720.0f * 3.0f + 720.0f;
	}

	if (TV_expansion_flag) {
		if (TV_size_x >= 2000 && TV_size_y >= 1130 && !on_flag) {
			off_flag = true;
			TV_expansion_flag = false;
		}

		TV_size_x += TV_expansion_x * deltaTime;
		TV_size_y += TV_expansion_y * deltaTime;
		TV_pos.x -= TV_expansion_x / 2 * deltaTime;
		TV_pos.y -= TV_expansion_y / 2.5f * deltaTime;
	}

	if (off_on_flag[off_on_MAX - 1]) {
		off_flag = false;
		on_flag = true;
		signal = 0;
	}

	if (on_flag) {
		if (launch <= 1) {
			launch += deltaTime;
		}
		else {
			background_frame_pos.x -= 3000.0f * deltaTime;
			if (background_frame_pos.x <= -1280.0f * 2) {
				background_frame_pos.x += 3840 - 1280;
			}
			if (launch_boss <= 1) {
				launch_boss += deltaTime;
			}
		}
		if (InputSystem.Keyboard.wasPressedThisFrame.Enter) {
			select_flag = true;
		}
	}
	if (select_flag) {
		launch_boss -= 2 * deltaTime;
		if (launch_select <= 1) {
			launch_select += deltaTime;
		}
		if (InputSystem.Keyboard.isPressed.Left) {
			select_ = 0;
		}
		if (InputSystem.Keyboard.isPressed.Right) {
			select_ = 1;
		}
		if (select_ == 0) {
			DontDestroy->attackAttribute = 1;
		}
		if (select_ == 1) {
			DontDestroy->attackAttribute = 2;
		}
			if (InputSystem.Keyboard.wasPressedThisFrame.Enter&&launch_select>=1) {
			VS_flag = true;
		}
	}
	if (VS_flag) {
		change_time += deltaTime;
		if (change_time >= 2) {
			off_flag = true;
			on_flag = false;
		}
		if (VS_pos.y <= 0) {
			VS_pos.y += 1000.0f * deltaTime;
		}
		if (red_pos.x <= -640) {
			red_pos.x += 1000.0f * deltaTime;
			player_pos.x += 1000.0f * deltaTime;
		}
		if (blue_pos.x >= 640) {
			blue_pos.x -= 1000.0f * deltaTime;
			enemy_pos.x -= 1000.0f * deltaTime;
		}
		if (off_on_flag[off_on_MAX - 1]) {
			return NextScene::MainScene;
		}
	}
	if (InputSystem.Keyboard.isPressed.T) {
		return NextScene::MainScene;
	}

	return NextScene::Continue;
}

// Draws the scene.
void titl::Render()
{
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);
	ID3D12DescriptorHeap* heaps[] = { des->Heap() };
	DXTK->CommandList->SetDescriptorHeaps((UINT)std::size(heaps), heaps);

	auto&& spriteBatch = sprite_batch_.get();
	spriteBatch->Begin(DXTK->CommandList);

	
	if (!off_flag) {
		sprite_batch_->Draw(
			error_sp.handle, error_sp.size, error_pos
		);

		sprite_batch_->Draw(
			TV_sp.handle, TV_sp.size, TV_pos
		);

		sprite_batch_->Draw(
			enter_sp.handle, enter_sp.size,
			Vector2(640, 360),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, signal),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad,//回転の角度
			Vector2(0, 0),//中心座標
			1.0f
		);

	}

	if (off_flag) {
		for (int i = 0; i < off_on_MAX; i++) {
			if (!off_on_flag[i]) {
				off_on_flag[i] = true;
				sprite_batch_->Draw(
					off_on_sp[i].handle, off_on_sp[i].size,
					Vector2(0, 0)
				);
				break;
			}
		}
	}


	if (on_flag) {
		sprite_batch_->Draw(
			Game_background_sp.handle, Game_background_sp.size,
			Vector2(0, 0),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, 1.0f),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad,//回転の角度
			Vector2(0, 0),//中心座標
			1.0f
		);

		sprite_batch_->Draw(
			background_frame_sp.handle, background_frame_sp.size,
			background_frame_pos,
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, launch),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad,//回転の角度
			Vector2(0, 0),//中心座標
			1.0f
		);

		sprite_batch_->Draw(
			boss_lush_sp.handle, boss_lush_sp.size,
			Vector2(240, 50),
			nullptr,//画像のどこに表示するか
			Color(1.0f, 1.0f, 1.0f, launch_boss),//色の間引き方(カラーマスク)
			0.0f * Mathf::Deg2Rad,//回転の角度
			Vector2(0, 0),//中心座標
			1.0f
		);
		if (select_flag) {
		
			sprite_batch_->Draw(
				background_sp.handle, background_sp.size,
				Vector2(0, 0),
				nullptr,//画像のどこに表示するか
				Color(1.0f, 1.0f, 1.0f, 0.5f),//色の間引き方(カラーマスク)
				0.0f * Mathf::Deg2Rad,//回転の角度
				Vector2(0, 0),//中心座標
				1.0f
			);
			if (select_ == 0) {
				sprite_batch_->Draw(
					select_arrow_sp.handle, select_arrow_sp.size,
					Vector2(70, 310)
				);
			}
			if (select_ == 1) {
				sprite_batch_->Draw(
				select_arrow_sp.handle, select_arrow_sp.size,
					Vector2(710, 310)
					);
			}
			sprite_batch_->Draw(
					player_sp.handle, player_sp.size,
					Vector2(170, 210)
				);
		sprite_batch_->Draw(
					player_sp.handle, player_sp.size,
					Vector2(810, 210),
					nullptr,//画像のどこに表示するか
					Colors::Red,//色の間引き方(カラーマスク)
					0.0f * Mathf::Deg2Rad,//回転の角度
					Vector2(0, 0),//中心座標
					1.0f
				);	
		sprite_batch_->Draw(
				select_sp.handle, select_sp.size,
				select_pos,
				nullptr,//画像のどこに表示するか
				Color(1.0f, 1.0f, 1.0f, launch_select),//色の間引き方(カラーマスク)
				0.0f * Mathf::Deg2Rad,//回転の角度
				Vector2(0, 0),//中心座標
				1.0f
			);

		}

		for (int i = off_on_MAX - 1; i >= 0; i--) {
			if (off_on_flag[i]) {
				off_on_flag[i] = false;
				sprite_batch_->Draw(
					off_on_sp[i].handle, off_on_sp[i].size,
					Vector2(0, 0)
				);
				break;
			}
		}

		sprite_batch_->Draw(
			red_sp.handle, red_sp.size,
			red_pos
		);

		sprite_batch_->Draw(
			blue_sp.handle, blue_sp.size,
			blue_pos
		);

		sprite_batch_->Draw(
			VS_sp.handle, VS_sp.size,
			VS_pos
		);
		if (DontDestroy->attackAttribute ==1) {
			sprite_batch_->Draw(
				player_sp.handle, player_sp.size,
				player_pos
			);
		}
		if (DontDestroy->attackAttribute ==2) {
			sprite_batch_->Draw(
				player_sp.handle, player_sp.size,
				player_pos,
				nullptr,//画像のどこに表示するか
				Colors::Red,//色の間引き方(カラーマスク)
				0.0f * Mathf::Deg2Rad,//回転の角度
				Vector2(0, 0),//中心座標
				1.0f
			);
		}
		sprite_batch_->Draw(
			enemy_sp.handle, enemy_sp.size,
			enemy_pos
		);
	}



	sprite_batch_->End();
	DXTK->ExecuteCommandList();
}
