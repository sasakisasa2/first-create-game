#pragma once

namespace ID 
{
	enum PlayerEnum
	{
		RedPlayer,
		BluePlayer,
		RedPlayerAttack,
		BluePlayerAttack,
		PlayerBit,
		PlayerBitAttack,
		PlayerCollisionSprite,
		PlayerCount
	};
	enum EnemyEnum 
	{
		Plant = PlayerCount,
		PlantAttack,
		EnemyCount
	};
	enum BossEnum 
	{
		Boss = EnemyCount,
		BossBullet,
		BossInductionBullet,
		BossShotBullet,
		BossBulletFrame,
		BossHP,
		BossHPFrame,
		BossCount
	};
	enum EffectEnum 
	{
		PlayerBitAttackEffect = BossCount,
		RedPlayerAttackEffect,
		BluePlayerAttackEffect,
		ExclamationMark,
		ExclamationTape,
		EffectCount
	};
	enum SpriteEnum
	{
		BG = EffectCount,
		TV,
		SpriteFont,
		Test,
		SpriteCount
	};
};