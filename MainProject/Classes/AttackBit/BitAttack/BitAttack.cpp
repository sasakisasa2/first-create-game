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
		//画像方向変換用
		SetRotationAngle(90.0f);
		//回転方向変換
		SetOmega(GetOmega() * -1);
	}

	if (attackDirection) {
		//曲がる方向変換
		SetAttackSpace(Vector2(GetAttackSpace().x*-1, GetAttackSpace().y));
	}

	for (int j = 1;j <= BIT_ATTACK_MAX;j++) {
		//初期角度から角度を変える
		attackTheta[j-1] = bitTheta - (GetAttackSpace().x * j );
	}
}

void  BitAttack::Update(Vector2 centerPosition) {
    #pragma region 色遊び
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
        #pragma region 回転移動
		//角度を増やす
		attackTheta[i] += GetOmega();

		//角度を参照し移動
		position[i]     = SquareMovement(attackTheta[i],//角度
									     centerPosition,//中心座標
			                             GetDistance() + GetAttackSpace().y * (i + 1));//円の範囲
        #pragma endregion

        #pragma region 画像詳細設定
		angle[i] = attackTheta[i] + GetRotationAngle();//画像回転
		/*color[i] = Vector4(colorNumber[0], colorNumber[1], colorNumber[2],1.0f);*/
		color[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);    //カラーの設定
        #pragma endregion
	}
}
