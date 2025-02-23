#pragma once
#include "../MainProject/Base/pch.h"
#include "../MainProject/Base/dxtk.h"
using namespace DirectX;
using namespace SimpleMath;
class BossManager {
private:
#pragma region Hpclass��
	float bossHpLow;//HP_MAX

#pragma endregion
	bool    isShotEnd;               //�U�����I��������ǂ����̔���

	Vector2 bossSpriteSize;          //�{�X�T�C�Y
	float   moveSpeed;               //�{�X�̓����X�s�[�h
	Vector2 normalSpriteSize;        //�ʏ�U���e�̑傫��
	float   normalShotSpeed;         //�ʏ�U���̃X�s�[�h
	float   normalShotPlace;         //�ʏ�e���m�̋���
	Vector2 aimShotSpriteSize;       //���@�_���e�̃T�C�Y
	float   aimShotPlaceMin;         //���@�_���e���ǂ��ɒu�����̍ŏ��l
	float   aimShotShotPlace;        //���@�_���e�ƒe�̊Ԋu
	float   aimShotShotHyde;         //���@�_���e���ǂ��̍����Ŏ~�߂邩
	float   aimShotSettingSpeed;     //���@�_���e�̒�~�ʒu�֍s���X�s�[�h
	float   aimShotShotSpeed;        //���@�_���e�������x
	float   toShotTime;	             //���@�_���e�̔��˂܂ł̎���
	Vector2 frameSize;               //�t���[���e�̃T�C�Y
	float   frameMoveSpeed;          //�t���[����]���x
	float   frameQuarter;            //�t���[���S�̐��̎l���̈�
	float   frameEighth;             //�t���[���S�̐��̔����̈�
	Vector2 inductionSpriteSize;     //�ǔ��e�̃T�C�Y
	float   inductionMoveSpeed;	     //�ǔ����x
	float   inductionLastAttackSpeed;//�ǔ��e�̍ŏI�U���̃X�s�[�h
	float   inductionAttackStopTime; //�ǔ��e�̍ŏI�U���܂ł̎���
	float   volumeLargeTime;         //�ǔ��e�̃T�C�Y��傫������܂ł̎���
	int     maxVolume;               //�ǔ��e�ő�T�C�Y

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
	static constexpr float HP_MAX                 = 2000;//�ő�HP
	static constexpr int   FRAME_BULLET_MAX       = 400; //�t���[���U���̒e��
	static constexpr int   NORMAL_SHOT_COUNT      = 4;   //�m�[�}���U���̉�
	static constexpr int   NORMAL_SHOT_BULLET_MAX = 60;  //�m�[�}���U���̒e��
	static constexpr int   AIMSHOT_COUNT          = 5;   //���@�_���U���̉�
	static constexpr int   AIMSHOT_BULLET_MAX     = 100;//���@�_���U���̒e��
	
	bool GetIsShotEnd() const { return isShotEnd; }
	void SetIsShotEnd(bool setFlag) { isShotEnd = setFlag; }

	//�͈͊O
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
