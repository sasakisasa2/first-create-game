//
// titl.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class titl final : public Scene {
public:
	titl();
	virtual ~titl() { Terminate(); }

	titl(titl&&) = default;
	titl& operator= (titl&&) = default;

	titl(titl const&) = delete;
	titl& operator= (titl const&) = delete;

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

	bool VS_flag = false;
	float change_time = 0;


	bool TV_expansion_flag = false;
	DirectXTK::Sprite TV_sp;
	SimpleMath::Vector2 TV_pos;
	float TV_size_x=1280;
	float TV_size_y = 720;
	float TV_expansion_x = 5.0f*100;
	float TV_expansion_y = 5.0f*56.25f;
	
	DirectXTK::Sprite error_sp;
	SimpleMath::Vector2 error_pos;


	DirectXTK::Sprite background_sp;

	static constexpr int off_on_MAX = 20;
	DirectXTK::Sprite off_on_sp[off_on_MAX];
	bool off_on_flag[off_on_MAX];
	bool off_flag = false;
	bool on_flag = false;


	DirectXTK::Sprite select_sp;
	SimpleMath::Vector2 select_pos;
	bool select_flag = false;
	float launch_select = 0;
	int select_ = 0;

	DirectXTK::Sprite select_arrow_sp;



	DirectXTK::Sprite background_frame_sp;
	SimpleMath::Vector2 background_frame_pos;

	DirectXTK::Sprite Game_background_sp;
	float launch=0;
	

	DirectXTK::Sprite enter_sp;
	float signal = 0;
	bool signal_flag = false;

	DirectXTK::Sprite boss_lush_sp;
	float launch_boss = 0;


	DirectXTK::Sprite red_sp;
	SimpleMath::Vector2 red_pos;

	DirectXTK::Sprite blue_sp;
	SimpleMath::Vector2 blue_pos;

	DirectXTK::Sprite VS_sp;
	SimpleMath::Vector2 VS_pos;

	DirectXTK::Sprite player_sp;
	SimpleMath::Vector2 player_pos;

	DirectXTK::Sprite enemy_sp;
	SimpleMath::Vector2 enemy_pos;

};