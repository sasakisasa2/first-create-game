#include"Move.h"

using namespace SimpleMath;

void  Move::Initialize()
{

	//�v���C���[�T�C�Y
	sprite.size.x = GetSpriteSize();
	sprite.size.y = GetSpriteSize();
}

void  Move::Update()
{

	Vector2 move = Vector2::Zero;	
	move = Move::MoveWASD (move);
	move = Move::MoveArrow(move);

#pragma region �������邩��
	////�_�b�V���̉���
	//int move_x = 1;
	////�_�b�V������
	//if (InputSystem.Keyboard.wasPressedThisFrame.A) {
	//	invincibleMove = true;
	//}

	////�_�b�V��
	//if (!invincibleMove && InputSystem.Keyboard.isPressed.A) {
	//	move_x = 2;
	//}
	////���G�_�b�V��
	//if (invincibleMove) {
	//	moveTime += delTime;
	//	move_x = 20;
	//	if (moveTime <= 0.02f) {
	//		invincibleMove = false;
	//		moveTime = 0;
	//	}
	//}
#pragma endregion
	move.Normalize();
	
	angle = CF::ChangeVectorToAngle(move, angle);
	
	//�ړ�
	position[0] += move * GetMoveSpeed() * DXTK->Time.deltaTime;

	//��ʊO�ɏo�����m�F
	position[0]  = PositionScreenOver(position[0]);
}

SimpleMath::Vector2 Move::ReturnPlayerPosition() 
{
	return position[0];
}

Vector2 Move::MoveWASD(Vector2 move)
{
	if (InputSystem.Keyboard.isPressed.D) { move += Vector2( 1.0f, 0.0f); }//�E�ړ�
	if (InputSystem.Keyboard.isPressed.A) { move += Vector2(-1.0f, 0.0f); }//���ړ�
	if (InputSystem.Keyboard.isPressed.S) { move += Vector2( 0.0f, 1.0f); }//���ړ�
	if (InputSystem.Keyboard.isPressed.W) { move += Vector2( 0.0f,-1.0f); }//��ړ�
	return move;
}

Vector2 Move::MoveArrow(Vector2 move)
{
	if (InputSystem.Keyboard.isPressed.Right) { move += Vector2( 1.0f, 0.0f);}//�E�ړ�
	if (InputSystem.Keyboard.isPressed.Left)  { move += Vector2(-1.0f, 0.0f);}//���ړ�
	if (InputSystem.Keyboard.isPressed.Down)  { move += Vector2( 0.0f, 1.0f);}//���ړ�
	if (InputSystem.Keyboard.isPressed.Up)    { move += Vector2( 0.0f,-1.0f);}//��ړ�
	return move;
}
