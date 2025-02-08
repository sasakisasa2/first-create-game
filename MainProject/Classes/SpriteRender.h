#pragma once

#include"..\Base\pch.h"
#include"..\Base\dxtk.h"

class SpriteRender {
private:

	DirectXTK::DescriptorHeap descHeap;
	DirectXTK::SpriteBatch    spriteBatch;
public:

#pragma region ”h¶ƒNƒ‰ƒX‚Ì‰æ‘œ•`‰æ—p
	RECT spriteRECT;
	DirectXTK::Sprite sprite;
	std::vector<SimpleMath::Vector2> position;
	std::vector<SimpleMath::Color> color;
	std::vector<float> angle;
#pragma endregion

	SpriteRender();

	void Load(int ID);
	void Render(int count);
};