//
// Scene.h
//

#pragma once

enum class NextScene : int {
	Continue = 0,
	MainScene,
	result,
	titl,
	SecondScene,
	StartScene = MainScene
};

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;

	Scene(Scene&&) = default;
	Scene& operator= (Scene&&) = default;

	Scene(Scene const&) = delete;
	Scene& operator= (Scene const&) = delete;

	virtual void Start() = 0;
	virtual void Initialize() = 0;
	virtual void LoadAssets() = 0;

	virtual void Terminate() = 0;

	virtual void OnDeviceLost() = 0;
	virtual void OnRestartSound() = 0;

	virtual NextScene Update(const float deltaTime) = 0;
	virtual void Render() = 0;
};