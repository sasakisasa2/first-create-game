#pragma once
#include"..\Boss\BossManager\BossManager.h"
#include"..\Boss\BossAttack\BossAttack.h"
#include"..\Boss\BossMove\BossMove.h"
#include"..\Boss\BossHp.h"
using namespace DirectX;
using namespace BossAttack;
class Boss : BossManager {
private:
    int       attackMode;
    BossMove  move;           //attackMode::2
    Normal    normalAttack[NORMAL_SHOT_COUNT];   //attackMode::2
    AimShot   aimShotAttack[5];//attackMode::4
    Frame     frameAttack;    //attackMode::1
    Induction inductionAttack;//attackMode::3
    CF::Timer modeChangeCoolTime;
    CF::Timer aimShotTimer[5];
    bool test = true;
    bool isNormalAttackShot[NORMAL_SHOT_COUNT];
    int aimShotMoveCount;
    int normalAttackCount;
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
