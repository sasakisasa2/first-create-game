//
// SecondScene.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class SecondScene final : public Scene {
public:
	SecondScene();
	virtual ~SecondScene() { Terminate(); }

	SecondScene(SecondScene&&) = default;
	SecondScene& operator= (SecondScene&&) = default;

	SecondScene(SecondScene const&) = delete;
	SecondScene& operator= (SecondScene const&) = delete;

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
	DirectXTK::DescriptorHeap des;
	DirectXTK::SpriteBatch    sprite_batch_;
	DirectXTK::SpriteFont spritefont;

	//îwåi
	DirectXTK::Sprite background_Sprite;

};