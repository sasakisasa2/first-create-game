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
		//‰æ‘œ•ûŒü•ÏŠ·—p
		SetRotationAngle(90.0f);
		//‰ñ“]•ûŒü•ÏŠ·
		SetOmega(GetOmega() * -1);
	}

	if (attackDirection) {
		//‹È‚ª‚é•ûŒü•ÏŠ·
		SetAttackSpace(Vector2(GetAttackSpace().x*-1, GetAttackSpace().y));
	}

	for (int j = 1;j <= BIT_ATTACK_MAX;j++) {
		//‰ŠúŠp“x‚©‚çŠp“x‚ð•Ï‚¦‚é
		attackTheta[j-1] = bitTheta - (GetAttackSpace().x * j );
	}
}

void  BitAttack::Update(Vector2 centerPosition) {
    #pragma region F—V‚Ñ
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
        #pragma region ‰ñ“]ˆÚ“®
		//Šp“x‚ð‘‚â‚·
		attackTheta[i] += GetOmega();

		//Šp“x‚ðŽQÆ‚µˆÚ“®
		position[i]     = SquareMovement(attackTheta[i],//Šp“x
									     centerPosition,//’†SÀ•W
			                             GetDistance() + GetAttackSpace().y * (i + 1));//‰~‚Ì”ÍˆÍ
        #pragma endregion

        #pragma region ‰æ‘œÚ×Ý’è
		angle[i] = attackTheta[i] + GetRotationAngle();//‰æ‘œ‰ñ“]
		/*color[i] = Vector4(colorNumber[0], colorNumber[1], colorNumber[2],1.0f);*/
		color[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);    //ƒJƒ‰[‚ÌÝ’è
        #pragma endregion
	}
}
