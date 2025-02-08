#include"BitMove.h"

using namespace SimpleMath;

void BitMove::Initialize(float theta, bool directionRotation) {

	sprite.size.x = GetSpriteSize().x;
	sprite.size.y = GetSpriteSize().y;

	if (directionRotation) {
		//�摜�����ϊ��p
		SetRotationAngle(90.0f);
		//��]�����ϊ�
		SetOmega(GetOmega() * -1);
	}

	bitTheta = theta;
}

void BitMove::Update(Vector2 centerPosition) {
	//�p�x���v���X����]������
	bitTheta      += GetOmega();
	//�p�x���Q�Ƃ����W��ύX
	position[0]    = SquareMovement(bitTheta, centerPosition, GetDistance());
	//�摜�̊p�x�ύX
	angle[0]       = bitTheta + GetRotationAngle();
}

