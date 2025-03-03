#include "BossAttack.h"

#pragma region Normal

void BossAttack::Normal::ShotMove(float moveVector)
{	
	int shotEndCount = 0;

	// 弾の移動
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{
		//設定方向へ移動
		position[bulletNumber] += ShotVectorSet(moveVector, NORMAL_SHOT_BULLET_MAX, bulletNumber) * 
								  GetNormalShotSpeed() * DXTK->Time.deltaTime;
		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			shotEndCount++;
			if (shotEndCount == NORMAL_SHOT_BULLET_MAX) { SetIsShotEnd(true); }
		}

	}
}

void BossAttack::Normal::ShotPreparation(Vector2 startPosition)
{
	//一回の攻撃で撃つ弾の数
	for (int bulletNumber = 0; bulletNumber < NORMAL_SHOT_BULLET_MAX; bulletNumber++)
	{	
		position[bulletNumber] = startPosition;
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

void BossAttack::AimShot::ShotReserve()
{
	isShotMove  = false;
	isPlaceMove = false;
	SetIsShotEnd(false);
}

Vector2 BossAttack::AimShot::SetAttackReserve(int bulletNumber)
{
	Vector2 setPosition;
	//準備位置の計算
	//x.(弾同士の間隔*(何発目か)+指定範囲までの距離),y.弾の高さ
	setPosition = Vector2(
		GetAimShotShotPlace() * bulletNumber + GetAimShotPlaceMin(),
		GetAimShotHyde());
	return setPosition;
}

void BossAttack::AimShot::Update(Vector2 bossPosition, Vector2 playerPosition)
{
	if (GetIsShotEnd()) { return; }
	
	//弾の発射までの時間を測る
	if (!isShotMove)
	{
		if (shotTimer.TimeMeasurement(GetToShotTime()))
		{
			//弾の発射
			isShotMove = true;
			shotTimer.TimerReSet();
		}
	}

	int endCount = 0;//攻撃が終わったかどうか判定するための変数
	
	//攻撃の弾数分回す
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
			reserveVector[bulletNumber] = CF::DistanceCount(SetAttackReserve(bulletNumber), position[bulletNumber]);
			//攻撃準備位置へ移動
			position[bulletNumber] += reserveVector[bulletNumber] * GetAimShotSettingSpeed() * DXTK->Time.deltaTime;
			angle[bulletNumber] = CF::ChangeVectorToAngle(reserveVector[bulletNumber], angle[bulletNumber]);
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
			angle[bulletNumber] = CF::ChangeVectorToAngle(shotVector[bulletNumber], angle[bulletNumber]);
		}
		//弾が範囲外にいるかどうかの確認
		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			endCount++;
			//攻撃全てが範囲外だった場合
			if (endCount == AIMSHOT_BULLET_MAX) { SetIsShotEnd(true); }
		}
	}
}
#pragma endregion

#pragma region Induction
BossAttack::Induction::Induction() :
	isMove(true), isPositionUpdate(true),
	isLastAttack(false),
	moveVector() , oldPlayerPosition(),
	volume(1)
{ } 

void BossAttack::Induction::InductionStart(Vector2 startPosition)
{
	volume			 = 1;
	isMove			 = true;
	isPositionUpdate = true;
	position[0]		 = startPosition;
	SetIsShotEnd(false);
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
		angle[0] = CF::ChangeVectorToAngle(CF::DistanceCount(oldPlayerPosition,position[0]), angle[0]);
		isPositionUpdate = false;
	}


	//最大サイズかつ一定時間後最終攻撃
	if (volume == GetMaxVolume())
	{
		//停止時間
		if (attackCoolTimer.TimeMeasurement(GetInductionAttackStopTime()))
		{
			isLastAttack = true;
			//ラストアタック
			position[0] += moveVector * GetInductionLastAttackSpeed() * DXTK->Time.deltaTime;
		}	
		else
		{
			isPositionUpdate = true;
			//発射まで方向を計算
			moveVector = CF::DistanceCount(oldPlayerPosition, position[0]);
		}

		if (CF::PositionRangeOver(position[0],XRange(), YRange()))
		{
			volume = 1;
			isMove = false;
			isPositionUpdate = false;
			SetIsShotEnd(true);
			attackCoolTimer.TimerReSet();
			largeTimer.TimerReSet();
		}
	}
}
#pragma endregion

#pragma region Frame

BossAttack::Frame::Frame(): move(), isVectorSwitch(false)
{
	xRange = Vector2((float)GetFrameSpriteSize().x / 2.0f, (float)DXTK->Screen.Width  - (float)GetFrameSpriteSize().x / 2.0f);
	yRange = Vector2((float)GetFrameSpriteSize().y / 2.0f, (float)DXTK->Screen.Height - (float)GetFrameSpriteSize().y / 2.0f);
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
				= Vector2(xRange.x,
		                  (yRange.y / GetFrameEighth()) * bulletNumber);
		}
		//弾最大数の8分の1を画面右端に設置
		else if (bulletNumber <= GetFrameQuarter())
		{
			setPosition 
				= Vector2(xRange.y,
				         (yRange.y / GetFrameEighth()) * (bulletNumber - GetFrameEighth()));
		}
		//残りの弾数の半分を画面上部に設置
		else if (bulletNumber <= (GetFrameQuarter() * 3.0f) - GetFrameEighth())
		{
			setPosition
				= Vector2((xRange.y / eighthPlusQuarter) * (bulletNumber - GetFrameQuarter()),
					       yRange.x);
		}
		//残りを画面下部に設置
		else
		{
			float frameNumber = (GetFrameQuarter() * 3.0f) - GetFrameEighth();
			setPosition
				= Vector2(xRange.y / eighthPlusQuarter *(bulletNumber - frameNumber),
					      yRange.y);
		}

		position[bulletNumber] = setPosition;

#pragma endregion
		//向かう方向を指定
		VectorSet(bulletNumber);
	}
}

void BossAttack::Frame::Update()
{
	int moveEndCount = 0;
	for (int bulletNumber = 0; bulletNumber < FRAME_BULLET_MAX; bulletNumber++) 
	{
		//指定方向移動
		position[bulletNumber] += move[bulletNumber] * GetFrameMoveSpeed() * DXTK->Time.deltaTime;
		angle[bulletNumber]     = CF::ChangeVectorToAngle(move[bulletNumber], angle[bulletNumber]);

		//切り返し	
		if(!isVectorSwitch)
		OutRange(bulletNumber);

		if (CF::PositionRangeOver(position[bulletNumber], XRange(), YRange()))
		{
			moveEndCount++;
			if (moveEndCount == FRAME_BULLET_MAX) { SetIsShotEnd(true); }
		}
	}
}

void BossAttack::Frame::OutRange(int number)
{
	bool rengeOver = CF::PositionRangeOver(position[number], xRange, yRange);
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
		reStartMove	     = position[number].y - yRange.x;
		position[number] = Vector2(xRange.y + reStartMove, yRange.x);
	}
	//上左端から下方向へ
	else if (move[number].x == -1.0f)
	{
		move[number] = Vector2(0.0f, 1.0f);
		reStartMove	     = xRange.x - position[number].x;
		position[number] = Vector2(xRange.x, yRange.x + reStartMove);
	}
	//下左端から右方向へ
	else if (move[number].y == 1.0f)
	{
		move[number]	 = Vector2(1.0f, 0.0f);
		reStartMove	     = position[number].y - yRange.y;
		position[number] = Vector2(xRange.x + reStartMove, yRange.y);
	}
	//下右端から上方向へ
	else if (move[number].x == 1.0f)
	{
		move[number] = Vector2(0.0f, -1.0f);
		reStartMove	     = xRange.y - position[number].x;
		position[number] = Vector2(xRange.y, yRange.y + reStartMove);
	}
}

void BossAttack::Frame::VectorSet(int number)
{
	//上部に居るとき左方向へ
	if (position[number].x <= xRange.y && position[number].y <= yRange.x)
	{
		move[number] = Vector2(-1.0f, 0.0f);
	}
	//左部に居るとき下方向へ
	if (position[number].x <= xRange.x && position[number].y >= yRange.x)
	{
		move[number] = Vector2(0.0f, 1.0f);
	}
	//下部に居るとき右方向へ
	if (position[number].x >= xRange.x && position[number].y >= yRange.y)
	{
		move[number] = Vector2(1.0f, 0.0f);
	}
	//右部に居るとき上方向へ
	if (position[number].x >= xRange.y && position[number].y <= yRange.y)
	{
		move[number] = Vector2(0.0f, -1.0f);
	}
}
#pragma endregion

