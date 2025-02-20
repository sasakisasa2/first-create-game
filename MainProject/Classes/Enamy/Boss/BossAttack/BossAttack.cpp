#include "BossAttack.h"

#pragma region Normal

void BossAttack::Normal::ShotMove(float moveVector)
{	
	// 弾の移動
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{
			position[bulletNumber] += ShotVectorSet(moveVector, NORMAL_SHOT_BULLET_MAX, bulletNumber) * 
									  GetNormalShotSpeed() * DXTK->Time.deltaTime;
	}
}

void BossAttack::Normal::ShotPreparation(Vector2 bossPosition)
{
	//一回の攻撃で撃つ弾の数
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{	
			 position[bulletNumber] = bossPosition;
	}
}

Vector2 BossAttack::Normal::ShotVectorSet(float shotVectorY,float shotMax, float bulletNumber)
{
	Vector2 vector;
	//全体数の半分から弾の配列の数字を割り、弾の方向を決める
	float place = bulletNumber / ((shotMax  / 2.0f));
	vector.x = -1.0f + place;
	if (place >= 1.0f)
	{
		place= 2.0f - place;
	}
	vector.y = place * shotVectorY;
	return vector;
}
#pragma endregion

#pragma region AimShot

BossAttack::AimShot::AimShot()
{
	for (int bulletNumber = 0; bulletNumber < AIMSHOT_COUNT; bulletNumber++)
	{
		isShotMove  = false;
		isPlaceMove = false;
	}
}

void BossAttack::AimShot::ShotReserve()
{
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{
		isShotMove  = false;
		isPlaceMove = false;
	}
}

Vector2 BossAttack::AimShot::SetAttackReserve(int bulletNumber)
{
	Vector2 setPosition;
	setPosition = Vector2(
		GetAimShotShotPlace() * (bulletNumber + 1.0f)  + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	//指定時間測る
	if (shotTimer.TimeMeasurement(GetToShotTime()))
	{
		//弾の残りの発射
		isShotMove = true;
		shotTimer.TimerReSet();
	}

	//攻撃一回の弾数
	for (int bulletNumber = 0; bulletNumber < AIMSHOT_BULLET_MAX; bulletNumber++)
	{
		//攻撃準備
		if (!isPlaceMove)
		{
			position[bulletNumber] = bossPosition;

			if (bulletNumber == AIMSHOT_BULLET_MAX-1)
				isPlaceMove = true;
		}
		//攻撃前の位置へ移動
		if (isPlaceMove && !isShotMove)
		{
			//攻撃準備位置の方向ベクトル
			Vector2 setMoveVector = CF::DistanceCount(SetAttackReserve(bulletNumber), position[bulletNumber]);
			//攻撃準備位置へ移動
			position[bulletNumber] += setMoveVector * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
		}
		if (!isShotMove)
		{
			//対象への方向を計算
			shotVector[bulletNumber] = CF::DistanceCount(playerPosition, position[bulletNumber]);
		}
		if (isShotMove)
		{
			//対象へ攻撃
			position[bulletNumber] += shotVector[bulletNumber] * GetAimShotShotSpeed() * DXTK->Time.deltaTime;
		}
	}
}
#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(true), isPositionUpdate(true),
	moveVector() , oldPlayerPosition(),
	volume(1)
{ } 

void BossAttack::Induction::InductionStart(Vector2 startPosition)
{
	volume			 = 1;
	isMove			 = true;
	isPositionUpdate = true;
	position[0]		 = startPosition;
	largeTimer.TimerReSet();
}

void BossAttack::Induction::Update(Vector2 playerPosition)
{
	//サイズの更新
	sprite.size.x = GetInductionSpriteSize().x * volume;
	sprite.size.y = GetInductionSpriteSize().y * volume;

	if (volume != GetMaxVolume())
	{	
		//発射まで方向を計算
		moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);

		//追跡移動	
		if (isMove)
		{
			position[0] += moveVector * GetInductionMoveSpeed() * volume * DXTK->Time.deltaTime;
		}
		//距離が近くなった際に追尾地点を更新
		if (CF::Distance(oldPlayerPosition, position[0]) <= 1)
		{
			isPositionUpdate = true;
		}

		//指定時間に達している場合サイズを大きくする
		if (largeTimer.TimeMeasurement(GetVolumeLargeTime()))
		{
			//サイズの拡大
			volume++;
			largeTimer.TimerReSet();
		}
	}

	//追尾地点の更新
	if (isPositionUpdate)
	{
		oldPlayerPosition = playerPosition;
		isPositionUpdate = false;
	}


	//最大サイズかつ一定時間後最終攻撃
	if (volume == GetMaxVolume())
	{
		//停止時間
		if (attackCoolTimer.TimeMeasurement(3))
		{
			//ラストアタック
			position[0] += moveVector * GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		}	
		else
		{
			isPositionUpdate = true;
			//発射まで方向を計算
			moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);
		}

		if (CF::PositionRengeOver(position[0],
			Vector2((float)sprite.size.x*-1.0f, (float)DXTK->Screen.Width  + (float)sprite.size.x),
			Vector2((float)sprite.size.y*-1.0f, (float)DXTK->Screen.Height + (float)sprite.size.y))
			)	
		{
			attackCoolTimer.TimerReSet();
			largeTimer.TimerReSet();
			volume = 1;
		}
	}
}
#pragma endregion

#pragma region Frame

BossAttack::Frame::Frame(): move()
{
	screenMin = Vector2(0.0f + GetFrameSpriteSize().x / 2.0f, 0.0f + GetFrameSpriteSize().y / 2.0f);
	screenMax = Vector2(DXTK->Screen.Width - GetFrameSpriteSize().x/2.0f, DXTK->Screen.Height - GetFrameSpriteSize().y/2.0f);
}

void BossAttack::Frame::PositionSet()
{	
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{	
#pragma region 最初の位置の設定

		float   eighthPlusQuarter = GetFrameEighth() + GetFrameQuarter();
		Vector2 setPosition;
		//弾最大数の8分の1を画面左端に設置
		if (bulletNumber <= GetFrameEighth())
		{
			setPosition 
				= Vector2(screenMin.x,
		                  (screenMax.y / GetFrameEighth()) * bulletNumber);
		}
		//弾最大数の8分の1を画面右端に設置
		else if (bulletNumber <= GetFrameQuarter())
		{
			setPosition 
				= Vector2(screenMax.x,
				         (screenMax.y / GetFrameEighth()) * (bulletNumber - GetFrameEighth()));
		}
		//残りの弾数の半分を画面上部に設置
		else if (bulletNumber <= (GetFrameQuarter() * 3.0f) - GetFrameEighth())
		{
			setPosition
				= Vector2((screenMax.x / eighthPlusQuarter) * (bulletNumber - GetFrameQuarter()),
					      screenMin.y);
		}
		//残りを画面下部に設置
		else
		{
			float frameNumber = (GetFrameQuarter() * 3.0f) - GetFrameEighth();
			setPosition
				= Vector2(screenMax.x / eighthPlusQuarter *(bulletNumber - frameNumber),
					      screenMax.y);
		}

		position[bulletNumber] = setPosition;

#pragma endregion
		//向かう方向を指定
		VectorSet(bulletNumber);
	}
}

void BossAttack::Frame::Update()
{
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{
		//指定方向移動
		position[bulletNumber] += move[bulletNumber] * GetFrameMoveSpeed() * DXTK->Time.deltaTime;
		//切り返し
		OutRenge(bulletNumber);
	}
}

void BossAttack::Frame::OutRenge(int number)
{
	bool rengeOver = CF::PositionRengeOver(position[number], screenMin, screenMax);
	//指定範囲外に出ていない場合戻す
	if (!rengeOver)
	{
		return;
	}	

    //進んでいた方向から時計回りの方向に切り返す
	float reStartMove;
	
	//上右方向から左方向へ
	if (move[number].y == -1.0f)
	{
		//指定範囲外から出た分指定方向に加え、フレームがガタガタにならないように
		move[number] = Vector2(-1.0f,0.0f);
		reStartMove	     = position[number].y - screenMin.y;
		position[number] = Vector2(screenMax.x + reStartMove , screenMin.y);
	}
	//上左端から下方向へ
	else if (move[number].x == -1.0f)
	{
		move[number] = Vector2(0.0f, 1.0f);
		reStartMove	     = screenMin.x - position[number].x;
		position[number] = Vector2(screenMin.x, screenMin.y + reStartMove);
	}
	//下左端から右方向へ
	else if (move[number].y == 1.0f)
	{
		move[number]	 = Vector2(1.0f, 0.0f);
		reStartMove	     = screenMax.y - position[number].y;
		position[number] = Vector2(screenMin.x - reStartMove, screenMax.y);
	}
	//下右端から上方向へ
	else if (move[number].x == 1.0f)
	{
		move[number] = Vector2(0.0f, -1.0f);
		reStartMove	     = screenMax.x - position[number].x;
		position[number] = Vector2(screenMax.x, screenMax.y + reStartMove);
	}
}

void BossAttack::Frame::VectorSet(int number)
{
	//上部に居るとき左方向へ
	if (position[number].x <= screenMax.x && position[number].y <= screenMin.y)
	{
		move[number] = Vector2(-1.0f, 0.0f);
	}
	//左部に居るとき下方向へ
	if (position[number].x <= screenMin.x && position[number].y >= screenMin.y)
	{
		move[number] = Vector2(0.0f, 1.0f);
	}
	//下部に居るとき右方向へ
	if (position[number].x >= screenMin.x && position[number].y >= screenMax.y)
	{
		move[number] = Vector2(1.0f, 0.0f);
	}
	//右部に居るとき上方向へ
	if (position[number].x >= screenMax.x && position[number].y <= screenMax.y)
	{
		move[number] = Vector2(0.0f, -1.0f);
	}
}
#pragma endregion

