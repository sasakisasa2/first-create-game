#include"BitAttack.h"
#include"..\BitMove\BitMove.h"

using namespace SimpleMath;

void  BitAttack::Initialize(float bitTheta, bool directionRotation, bool attackDirection) {

	sprite.size.x = GetAttackSpriteSize().x;
	sprite.size.y = GetAttackSpriteSize().y;

	for (int i = 0; i < 3; i++) {
		colorNumber[i] = 0.1f * i;
		colorNumberFlag[i] = false;
	}

	if (directionRotation) {
		//�摜�����ϊ��p
		SetRotationAngle(90.0f);
		//��]�����ϊ�
		SetOmega(GetOmega() * -1);
	}

	if (attackDirection) {
		//�Ȃ�������ϊ�
		SetAttackSpace(Vector2(GetAttackSpace().x*-1, GetAttackSpace().y));
	}

	for (int j = 1;j <= BIT_ATTACK_MAX;j++) {
		//�����p�x����p�x��ς���
		attackTheta[j-1] = bitTheta - (GetAttackSpace().x * j );
	}
}

void  BitAttack::Update(Vector2 centerPosition) {
    #pragma region �F�V��
	for (int i = 0; i < 3; i++) {
		if (!colorNumberFlag[i]) {
			colorNumber[i] += DXTK->Time.deltaTime * i;
			if (colorNumber[i] >= 1) {
				colorNumberFlag[i] = true;
			}
		}
		if (colorNumberFlag[i]) {
			colorNumber[i] -= DXTK->Time.deltaTime / i;
			if (colorNumber[i] <= 0) {
				colorNumberFlag[i] = false;
			}
		}
	}
    #pragma endregion

	for (int i = 0; i < BIT_ATTACK_MAX; i++) {
        #pragma region ��]�ړ�
		//�p�x�𑝂₷
		attackTheta[i] += GetOmega();

		//�p�x���Q�Ƃ��ړ�
		position[i]     = SquareMovement(attackTheta[i],//�p�x
									     centerPosition,//���S���W
			                             GetDistance() + GetAttackSpace().y * (i + 1));//�~�͈̔�
        #pragma endregion

        #pragma region �摜�ڍאݒ�
		angle[i] = attackTheta[i] + GetRotationAngle();//�摜��]
		/*color[i] = Vector4(colorNumber[0], colorNumber[1], colorNumber[2],1.0f);*/
		color[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);    //�J���[�̐ݒ�
        #pragma endregion
	}
}
