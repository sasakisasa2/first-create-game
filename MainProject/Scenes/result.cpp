//
// result.cpp
//

#include "..\Base\pch.h"
#include "..\Base\dxtk.h"
#include "SceneFactory.h"

using namespace SimpleMath;

// Initialize member variables.
result::result()
{

}

// Start is called after the scene is created.
void result::Start()
{
	LoadAssets();
	Initialize();
}

// Allocate all memory the Direct3D and Direct2D resources.
void result::LoadAssets()
{

}

// Initialize a variable and audio resources.
void result::Initialize()
{

}

// Releasing resources required for termination.
void result::Terminate()
{
	// TODO: Add a sound instance reset.
	DXTK->Audio.Engine->Suspend();


	DXTK->Audio.ResetEngine();
	DXTK->WaitForGpu();

	// TODO: Add your Termination logic here.

}

// Direct3D resource cleanup.
void result::OnDeviceLost()
{

}

// Restart any looped sounds here
void result::OnRestartSound()
{

}

// Updates the scene.
NextScene result::Update(const float deltaTime)
{
	// If you use 'deltaTime', remove it.
	UNREFERENCED_PARAMETER(deltaTime);

	// TODO: Add your game logic here.



	return NextScene::Continue;
}

// Draws the scene.
void result::Render()
{
	// TODO: Add your rendering code here.
	DXTK->ResetCommand();
	DXTK->ClearRenderTarget(Colors::CornflowerBlue);



	DXTK->ExecuteCommandList();
}
