#include"Move.h"

using namespace SimpleMath;

void  Move::Initialize()
{
	//プレイヤーサイズ
	sprite.size.x = GetSpriteSize();
	sprite.size.y = GetSpriteSize();
}

void  Move::Update()
{
	moveVector = Vector2::Zero;
	moveVector = Move::MoveWASD (moveVector);
	moveVector = Move::MoveArrow(moveVector);

#pragma region 実装するかも
	////ダッシュの加速
	//int move_x = 1;
	////ダッシュ発動
	//if (InputSystem.Keyboard.wasPressedThisFrame.A) {
	//	invincibleMove = true;
	//}

	////ダッシュ
	//if (!invincibleMove && InputSystem.Keyboard.isPressed.A) {
	//	move_x = 2;
	//}
	////無敵ダッシュ
	//if (invincibleMove) {
	//	moveTime += delTime;
	//	move_x = 20;
	//	if (moveTime <= 0.02f) {
	//		invincibleMove = false;
	//		moveTime = 0;
	//	}
	//}
#pragma endregion
	moveVector.Normalize();
	
	const float NINTY = 90;

	angle[0] = NINTY;
	
	//移動
	position[0] += moveVector * GetMoveSpeed() * DXTK->Time.deltaTime;

	//画面外に出たか確認
	position[0]  = PositionScreenOver(position[0]);
}

Vector2 Move::MoveWASD(Vector2 move)
{
	if (InputSystem.Keyboard.isPressed.D) { move += Vector2( 1.0f, 0.0f); }//右移動
	if (InputSystem.Keyboard.isPressed.A) { move += Vector2(-1.0f, 0.0f); }//左移動
	if (InputSystem.Keyboard.isPressed.S) { move += Vector2( 0.0f, 1.0f); }//下移動
	if (InputSystem.Keyboard.isPressed.W) { move += Vector2( 0.0f,-1.0f); }//上移動
	return move;
}

Vector2 Move::MoveArrow(Vector2 move)
{
	if (InputSystem.Keyboard.isPressed.Right) { move += Vector2( 1.0f, 0.0f);}//右移動
	if (InputSystem.Keyboard.isPressed.Left)  { move += Vector2(-1.0f, 0.0f);}//左移動
	if (InputSystem.Keyboard.isPressed.Down)  { move += Vector2( 0.0f, 1.0f);}//下移動
	if (InputSystem.Keyboard.isPressed.Up)    { move += Vector2( 0.0f,-1.0f);}//上移動
	return move;
}
