//
// SecondScene.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"

using namespace SimpleMath;

// Initialize member variables.
SecondScene::SecondScene()
{

}

// Start is called after the scene is created.
void SecondScene::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void SecondScene::LoadAssets()
{
	auto&& device = DXTK->Device;
	des = DirectXTK::CreateDescriptorHeap(device, 200);

	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	//ƒvƒŒƒCƒ„[‰æ‘œ
	background_Sprite = DirectXTK::CreateSpriteSRV(
		device, L"frame.png", resourceUpload,
		des, 0
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
void SecondScene::Initialize()
{
	background_Sprite.size.x = 1280;
	background_Sprite.size.y = 720;




}

// Releasing resources required for termination.
void SecondScene::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void SecondScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void SecondScene::OnRestartSound()
{

}

// Updates the scene.
NextScene SecondScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.



	return NextScene::Continue;
}

// Draws the scene.
void SecondScene::Render()
{
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);
	ID3D12DescriptorHeap* heaps[] = { des->Heap() };
	DXTK->CommandList->SetDescriptorHeaps((UINT)std::size(heaps), heaps);

	auto&& spriteBatch = sprite_batch_.get();
	spriteBatch->Begin(DXTK->CommandList);

	sprite_batch_->Draw(
		background_Sprite.handle, background_Sprite.size, Vector2(0, 0)
	);



	DXTK->ExecuteCommandList();
}
