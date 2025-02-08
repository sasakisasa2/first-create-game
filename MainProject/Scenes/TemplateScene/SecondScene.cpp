//
// TemplateScene.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"

using namespace SimpleMath;

// Initialize member variables.
TemplateScene::TemplateScene()
{

}

// Start is called after the scene is created.
void TemplateScene::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void TemplateScene::LoadAssets()
{

}

// Initialize a variable and audio resources.
void TemplateScene::Initialize()
{

}

// Releasing resources required for termination.
void TemplateScene::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void TemplateScene::OnDeviceLost()
{

}

// Restart any looped sounds here
void TemplateScene::OnRestartSound()
{

}

// Updates the scene.
NextScene TemplateScene::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.



	return NextScene::Continue;
}

// Draws the scene.
void TemplateScene::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);



	DXTK->ExecuteCommandList();
}
