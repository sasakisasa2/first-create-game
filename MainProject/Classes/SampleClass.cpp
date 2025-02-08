//#include"Sample.h"
//
//using namespace SimpleMath;
//
//void Sample::LoadAssets(DirectXTK::Sprite preSprite) {
//	sprite = preSprite;
//}
//
//void  Sample::Initialize() {
//	//プレイヤーサイズ
//	sprite.size.x = size;
//	sprite.size.y = size;
//	position = Vector2(300, 300);
//}
//
//void  Sample::Update(float deltaTime) {
//
//}
//
//void Sample::Render(DirectX::SpriteBatch* SpriteBatch) {
//
//	//プレイヤー画像呼び出し
//	SpriteBatch->Draw(
//		sprite.handle, sprite.size, position,
//		nullptr,//画像のどこに表示するか
//		Colors::White,//色の間引き方(カラーマスク)
//		0.0f * Mathf::Deg2Rad, Vector2(size / 2, size / 2),//回転の角度と軸
//		1.0f
//	);
//}
//
