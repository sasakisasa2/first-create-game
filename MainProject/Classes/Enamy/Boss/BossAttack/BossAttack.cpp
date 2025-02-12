#include "BossAttack.h"

#pragma region Normal

BossAttack::Normal::Normal():isMoveStart(false), count(0)
{
	//全体の攻撃回数
	for (int attackCount = 0; attackCount < NORMAL_SHOT_COUNT; attackCount++)
	{
		//攻撃回数に応じた最初の弾の配列の数
		int numberCountMax = normalOneShot * attackCount;

		//弾の撃つ方向
		float shotVectorY = -1.0f;
		//攻撃回数から上方向か下方向か判定する
		if (attackCount % 2 == 0) 
		{
			  shotVectorY = 1.0f;
		}

		//一回の攻撃で撃つ弾の数
		for (int bulletNumber = 0; bulletNumber < NORMAL_ONESHOT_MAX; bulletNumber++)
		{
			isShot[bulletNumber + numberCountMax]= false;

			moveVector[bulletNumber + numberCountMax]
				= ShotVectorSet(shotVectorY, NORMAL_SHOT_BULLET_MAX, NORMAL_SHOT_COUNT, bulletNumber);
		}
	}
}


void BossAttack::Normal::ShotMove()
{	
	// 弾の移動
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{
		if (isShot[bulletNumber]) 
		{
			position[bulletNumber] += moveVector[bulletNumber] * GetNormalShotSpeed() * DXTK->Time.deltaTime;
		}
	}
}

void BossAttack::Normal::ShotPreparation(Vector2 bossPosition)
{
	if (isMoveStart) { return; }
	//一回の攻撃で撃つ弾の数
	for (int bulletNumber = 0; bulletNumber < NORMAL_ONESHOT_MAX; bulletNumber++)
	{	
			 isShot[bulletNumber+ (NORMAL_ONESHOT_MAX * count)] = true;
			 position[bulletNumber + (NORMAL_ONESHOT_MAX * count)] = bossPosition;

		if (bulletNumber == NORMAL_ONESHOT_MAX - 1) 
		{
			count++;
			isMoveStart = true; 
			break;
		}
	}
}

Vector2 BossAttack::Normal::ShotVectorSet(float shotVectorY,float shotMax,float shotCountMax, float bulletNumber)
{
	Vector2 vector;
	//全体数の半分から弾の配列の数字を割り、弾の方向を決める
	float place = bulletNumber / (((shotMax / shotCountMax) / 2.0f)-1);
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
		isShotMove[bulletNumber]  = 0;
		isPlaceMove[bulletNumber] = 0;
	}
}

void BossAttack::AimShot::ShotReserve()
{
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{
		isShotMove[attackCount]  = false;

		isPlaceMove[attackCount] = false;
	}
}

Vector2 BossAttack::AimShot::SetAttackReserve(int attackCount, int bulletNumber)
{
	Vector2 setPosition;
	setPosition = Vector2(
		GetAimShotShotPlace() * ((bulletNumber - AIMSHOT_ONEATTACK_BULLET_MAX * attackCount) + 1.0f) + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	//自機狙い弾
	Vector2 setPositionMove;
	//指定時間測る
	bool isTimer = shotTimer.TimeMeasurement(GetToShotTime());
	//攻撃回数
	for (int attackCount = 0; attackCount < AIMSHOT_COUNT; attackCount++)
	{		
		//弾数
		for (int bulletNumber = AIMSHOT_ONEATTACK_BULLET_MAX * attackCount; bulletNumber < AIMSHOT_ONEATTACK_BULLET_MAX * (attackCount + 1.0f); bulletNumber++)
		{
			//攻撃準備
			if (!isPlaceMove[attackCount])
			{
				position[bulletNumber] = bossPosition;

				if (bulletNumber == AIMSHOT_ONEATTACK_BULLET_MAX - 1)
					isPlaceMove[attackCount] = true;
			}
			//攻撃前の位置へ移動
			if (isPlaceMove[attackCount] && !isShotMove[attackCount])
			{
				//攻撃準備位置の方向ベクトル
				setPositionMove = CF::DistanceCount(SetAttackReserve(attackCount, bulletNumber), position[bulletNumber]);
				//攻撃準備位置へ移動
				position[bulletNumber] += setPositionMove * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
			}
			if (!isShotMove[attackCount])
			{
				//対象へのベクトルを計算
				shotVector[bulletNumber] = CF::DistanceCount(playerPosition, position[bulletNumber]);
			}
			if (isShotMove[attackCount])
			{
				//対象へ攻撃
				position[bulletNumber] += shotVector[bulletNumber] * GetAimShotShotSpeed() * DXTK->Time.deltaTime;
			}
		}
		if (isTimer)
		{
			if (attackCount == AIMSHOT_COUNT - 1.0f)
			{
				//弾の残りの発射
				isShotMove[attackCount] = true;
				shotTimer.TimerReSet();
			}
			else
			{
				//弾の半分の発射と同時に次の弾の準備
				isShotMove[attackCount] = true;
				isPlaceMove[attackCount + 1] = true;
				shotTimer.TimerReSet();
			}
		}
		else 
		{
			break;
		}
	}
	//画面外に出たら初期化
	if (CF::PositionRengeOver(position[AIMSHOT_BULLET_MAX - 1],
		Vector2(-sprite.size.x, DXTK->Screen.Width + sprite.size.x),
		Vector2(-sprite.size.y, DXTK->Screen.Height + sprite.size.y)))
	{
		for (int bulletNumber = 0; bulletNumber < AIMSHOT_COUNT; bulletNumber++)
		{
			isShotMove[bulletNumber]  = false;
			isPlaceMove[bulletNumber] = false;
		}
	}
}

#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(false), isPositionUpdate(true),
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
	
	bool isLarge = largeTimer.TimeMeasurement(GetVolumeLargeTime());

	//距離が近くなった際に追尾地点を更新
	if (CF::Distance(oldPlayerPosition,position[0]) <= 1) 
	{
		isPositionUpdate = true;
	}

	//追尾地点の更新
	if (isPositionUpdate || volume == GetMaxVolume() && isLarge)
	{
		oldPlayerPosition = playerPosition;
		isPositionUpdate = false;
	}

	//最大サイズかつ一定時間後最終攻撃
	if (volume == GetMaxVolume() && isLarge)
	{
		isMove = false;
		position[0] += CF::DistanceCount(oldPlayerPosition, position[0]) *
					   GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		if (CF::PositionRengeOver (position[0],
				 Vector2(-sprite.size.x, DXTK->Screen.Width  + sprite.size.x),
				 Vector2(-sprite.size.y, DXTK->Screen.Height + sprite.size.y)))
		{
			largeTimer.TimerReSet();
			volume = 1;
		}
	}

	//追跡移動
	if (isMove) 
	{
		position[0] += CF::DistanceCount(oldPlayerPosition, position[0])*
					   GetInductionMoveSpeed() * volume * DXTK->Time.deltaTime;
	}
	//大きさ変化
	if (volume != GetMaxVolume() && isLarge)
	{
		volume++;
		largeTimer.TimerReSet();
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

