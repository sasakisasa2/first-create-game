#pragma once
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
class BossManager {
private:
#pragma region Hpclass側
	float bossHpLow;//HP_MAX

#pragma endregion
	bool    isShotEnd;               //攻撃が終わったかどうかの判定

	Vector2 bossSpriteSize;          //ボスサイズ
	float   moveSpeed;               //ボスの動くスピード
	Vector2 normalSpriteSize;        //通常攻撃弾の大きさ
	float   normalShotSpeed;         //通常攻撃のスピード
	float   normalShotPlace;         //通常弾同士の距離
	Vector2 aimShotSpriteSize;       //自機狙い弾のサイズ
	float   aimShotPlaceMin;         //自機狙い弾をどこに置くかの最小値
	float   aimShotShotPlace;        //自機狙い弾と弾の間隔
	float   aimShotShotHyde;         //自機狙い弾をどこの高さで止めるか
	float   aimShotSettingSpeed;     //自機狙い弾の停止位置へ行くスピード
	float   aimShotShotSpeed;        //自機狙い弾を撃つ速度
	float   toShotTime;	             //自機狙い弾の発射までの時間
	Vector2 frameSize;               //フレーム弾のサイズ
	float   frameMoveSpeed;          //フレーム回転速度
	float   frameQuarter;            //フレーム全体数の四分の一
	float   frameEighth;             //フレーム全体数の八分の一
	Vector2 inductionSpriteSize;     //追尾弾のサイズ
	float   inductionMoveSpeed;	     //追尾速度
	float   inductionLastAttackSpeed;//追尾弾の最終攻撃のスピード
	float   inductionAttackStopTime; //追尾弾の最終攻撃までの時間
	float   volumeLargeTime;         //追尾弾のサイズを大きくするまでの時間
	int     maxVolume;               //追尾弾最大サイズ

protected: 
#pragma region getter
	Vector2 GetBossSpriteSize()          const { return bossSpriteSize;           }
	float   GetMoveSpeed()               const { return moveSpeed;                }
	Vector2 GetNormalSpriteSize()        const { return normalSpriteSize;         }
	float   GetNormalShotSpeed()         const { return normalShotSpeed;          }
	float   GetNormalShotPlace()         const { return normalShotPlace;          }
	float   GetToShotTime()	             const { return toShotTime;               }
	Vector2 GetAimShotSpriteSize()	     const { return aimShotSpriteSize;	      }
	float   GetAimShotPlaceMin()         const { return aimShotPlaceMin;          }
	float   GetAimShotShotPlace()        const { return aimShotShotPlace;         }
	float   GetAimShotHyde()             const { return aimShotShotHyde;          }
	float   GetAimShotSettingSpeed()     const { return aimShotSettingSpeed;      }
	float   GetAimShotShotSpeed()	     const { return aimShotShotSpeed;         }
	Vector2 GetInductionSpriteSize()     const { return inductionSpriteSize;      }
	float   GetInductionMoveSpeed()	     const { return inductionMoveSpeed;       }
	float   GetInductionLastAttackSpeed()const { return inductionLastAttackSpeed; }
	float   GetInductionAttackStopTime() const { return inductionAttackStopTime;  }
	float   GetVolumeLargeTime()         const { return volumeLargeTime;          }
	int     GetMaxVolume()               const { return maxVolume;                }
	Vector2 GetFrameSpriteSize()         const { return frameSize;                }
	float   GetFrameMoveSpeed()          const { return frameMoveSpeed;           }
	float   GetFrameQuarter()            const { return frameQuarter;             }
	float   GetFrameEighth()             const { return frameEighth;              }
#pragma endregion


	DirectXTK::Sprite    sprite;
	std::vector<Vector2> position;
	std::vector<Color>   color;
	std::vector<float>   angle;
	DX12::SpriteEffects  effect;

	BossManager();
public:	
	static constexpr float HP_MAX                 = 2000;//最大HP
	static constexpr int   FRAME_BULLET_MAX       = 400; //フレーム攻撃の弾数
	static constexpr int   NORMAL_SHOT_COUNT      = 4;   //ノーマル攻撃の回数
	static constexpr int   NORMAL_SHOT_BULLET_MAX = 60;  //ノーマル攻撃の弾数
	static constexpr int   AIMSHOT_COUNT          = 5;   //自機狙い攻撃の回数
	static constexpr int   AIMSHOT_BULLET_MAX     = 100;//自機狙い攻撃の弾数
	
	bool GetIsShotEnd() const { return isShotEnd; }
	void SetIsShotEnd(bool setFlag) { isShotEnd = setFlag; }

	//範囲外
	Vector2 XRange()
	{
		Vector2 xRange = Vector2((float)sprite.size.x / -2.0f, (float)DXTK->Screen.Width + (float)sprite.size.x / 2.0f);
		return xRange;
	}

	Vector2 YRange() const
	{
		Vector2 yRange = Vector2((float)sprite.size.y / -2.0f, (float)DXTK->Screen.Width + (float)sprite.size.y / 2.0f);
		return yRange;
	}

    void Load(DirectXTK::Sprite& preSprite);
	void SpriteSizeSet(Vector2 setSize) 
	{
		sprite.size.x = setSize.x;
		sprite.size.y = setSize.y;
	}
    void Render(DirectX::SpriteBatch* SpriteBatch,int count);
};
