//
// SceneFactory.h
//

#pragma once

#include "MainScene.h"
#include "titl.h"
#include "SecondScene.h"
#include "result.h"


class SceneFactory final {
public:
	static std::unique_ptr<Scene> CreateScene(const NextScene nextScene)
	{
		std::unique_ptr<Scene> scene;
		switch (nextScene) {
		case NextScene::MainScene:	scene = std::make_unique<MainScene>();	break;
		case NextScene::titl:	scene = std::make_unique<titl>();	break;
		case NextScene::result:	scene = std::make_unique<result>();	break;
		case NextScene::SecondScene:	scene = std::make_unique<SecondScene>();	break;
		}
		return scene;
	}
};