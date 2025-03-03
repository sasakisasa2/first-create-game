#pragma once
#include"..\Boss\BossManager\BossManager.h"
#include"..\Boss\BossAttack\BossAttack.h"
#include"..\Boss\BossMove\BossMove.h"
#include"..\Boss\BossHp.h"
using namespace DirectX;
using namespace BossAttack;
class Boss : public BossManager {
private:
    int       attackMode;

    BossMove  move;                           //attaxkMode::1
    Frame     frameAttack;                    //attackMode::1
    Normal    normalAttack[NORMAL_SHOT_COUNT];//attackMode::1
    Induction inductionAttack;                //attackMode::2
    AimShot   aimShotAttack[AIMSHOT_COUNT];   //attackMode::3

    CF::Timer aimShotTimer[AIMSHOT_COUNT];
    bool isNormalAttackShot[NORMAL_SHOT_COUNT];
    int  aimShotMoveCount;
    int  normalAttackCount;

    CollisionInfo bossCollisionInfo;
    CollisionInfo frameAttackCollisionInfo;
    CollisionInfo normalAttackCollisionInfo[NORMAL_SHOT_COUNT * NORMAL_SHOT_BULLET_MAX];
    CollisionInfo inductionAttackCollisionInfo;
    CollisionInfo frameAttackCollisionInfo[AIMSHOT_COUNT * AIMSHOT_BULLET_MAX];

public:
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
