#pragma once
#include"..\Boss\BossManager\BossManager.h"
#include"..\Boss\BossAttack\BossAttack.h"
#include"..\Boss\BossMove\BossMove.h"
#include"..\Boss\BossHp.h"
#include"..\MainProject\Classes\collision\CollisionInfo\CollisionInfo.h"
using namespace DirectX;
using namespace BossAttack;
class Boss : public BossManager {
private:
    int       attackMode;

    BossMove  move;                           //attackMode::1
    Frame     frameAttack;                    //attackMode::1
    Normal    normalAttack[NORMAL_SHOT_COUNT];//attackMode::1
    Induction inductionAttack;                //attackMode::2
    AimShot   aimShotAttack[AIMSHOT_COUNT];   //attackMode::3

    CF::Timer aimShotTimer[AIMSHOT_COUNT];
    bool isNormalAttackShot[NORMAL_SHOT_COUNT];
    int  aimShotMoveCount;
    int  normalAttackCount;

    CollisionInfo bossCollisionInfo;
    CollisionInfo frameCollisionInfo[FRAME_BULLET_MAX];
    CollisionInfo normalCollisionInfo[NORMAL_SHOT_COUNT * NORMAL_SHOT_BULLET_MAX];
    CollisionInfo inductionCollisionInfo;
    CollisionInfo AimShotCollisionInfo[AIMSHOT_COUNT * AIMSHOT_BULLET_MAX];

public:
    //CollisionInfo bossCollisionInfo() { return bossCollisionInfo; };
    //CollisionInfo GetFrameCollisionInfo(int bulletNumber) { return frameCollisionInfo[bulletNumber]; };
    //CollisionInfo GetNormalCollisionInfo(int bulletNumber) { return normalCollisionInfo[bulletNumber]; };
    //CollisionInfo GetInductionCollisionInfo() { return inductionCollisionInfo; };
    //CollisionInfo GetAimShotCollisionInfo(int bulletNumber) { return AimShotCollisionInfo[bulletNumber]; };


    Boss() :attackMode(1), normalAttackCount(0), aimShotMoveCount(1), isNormalAttackShot()
    { }
    void Load(DirectXTK::Sprite& bossSprite,
              DirectXTK::Sprite& atttackSprite,
              DirectXTK::Sprite& hpSprite,
              DirectXTK::Sprite& hpBackGroundSprite);
    void Initialize();
    void Update(Vector2 playerPosition);
    void Render(DirectX::SpriteBatch* SpriteBatch);

};
