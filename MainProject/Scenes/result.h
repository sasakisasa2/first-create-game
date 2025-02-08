//
// result.h
//

#pragma once

#include "Scene.h"

using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::make_unique;
using namespace DirectX;

class result final : public Scene {
public:
	result();
	virtual ~result() { Terminate(); }

	result(result&&) = default;
	result& operator= (result&&) = default;

	result(result const&) = delete;
	result& operator= (result const&) = delete;

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

};