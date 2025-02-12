#pragma once
#include "..\BossManager\BossManager.h"
using namespace DirectX;
using namespace SimpleMath;
namespace BossAttack
{
    class Normal:public BossManager
    {
    private:
        bool    isMoveStart;
        int     count;
        bool    isShot    [NORMAL_SHOT_BULLET_MAX];//’e‚Ì”­ËŠÇ—
        Vector2 moveVector[NORMAL_SHOT_BULLET_MAX];//UŒ‚•ûŒü
        int     normalOneShot = NORMAL_ONESHOT_MAX;
        /// <summary>
        /// Å‘å”‚©‚çŒ»İ‚ª‰½”­–Ú‚©‚É‰‚¶‚ÄŒü‚©‚¤•ûŒü‚ğ•Ô‚·
        /// ”¼‰~ó‚É“WŠJ‚·‚é
        /// </summary>
        /// <param name="shotVectorY">‰º•ûŒü‚©ã•ûŒü‚©(ã::-1,‰º::1)</param>
        /// <param name="oneShotBulletMax">’e‚ÌÅ‘å”</param>
        /// <param name="bulletNumber">Œ»İ‰½”­–Ú‚©</param>
        /// <returns>Œü‚©‚¤•ûŒü</returns>
        Vector2 ShotVectorSet(float shotVectorY,float shotMax,float shotCountMax,float bulletNumber);

    public:
        Normal();
        int  GetAttackCount() const { return count; }
        bool GetIsMoveStart() const { return isMoveStart; }
        void SetIsMoveStart(bool setIsMove) { isMoveStart = setIsMove; }
        void ShotMove();
        void ShotPreparation(Vector2 bossPosition);
    };

    class AimShot :public BossManager
    {
    private:
        CF::Timer shotTimer;
        Vector2 shotVector[AIMSHOT_BULLET_MAX];
        //”­Ë‚ğŠÇ—‚·‚é•Ï”
        bool isShotMove[AIMSHOT_COUNT];
        bool isPlaceMove[AIMSHOT_COUNT];//’e‚Ì’èˆÊ’u‚Ü‚Å‚ÌˆÚ“®ŠÇ—


        Vector2 SetAttackReserve(int attackCount,int bulletNumber);

    public:
        AimShot();
        /// <summary>
        /// ‰Šú‰»ˆ—
        /// </summary>
        void ShotReserve();
        void Update(Vector2 BossPosition,Vector2 playerPosition);
    };

    class Induction :public BossManager
    {
    private:
        CF::Timer largeTimer;
        SimpleMath::Vector2 moveVector;
        SimpleMath::Vector2 oldPlayerPosition;
        //“r’†‚Å’Ç”ö‚ğ‚â‚ßis‚é
        bool isMove;
        bool isPositionUpdate;
        //’e‚Ì‘å‚«‚³
        int volume;
    public:
        Induction();

        /// <summary>
        /// ’Ç”ö’e‚Ì‹N“®
        /// </summary>
        /// <param name="startPosition">’Ç”ö‚Ìn‚Ü‚éƒ|ƒCƒ“ƒg</param>
        void InductionStart(Vector2 startPosition);

        /// <summary>
        /// ’Ç”ö
        /// </summary>
        /// <param name="playerPosition">’Ç”ö‘ÎÛ</param>
        void Update(Vector2 playerPosition);

    };

    class Frame :public BossManager
    {
    private:
        Vector2 move[FRAME_BULLET_MAX];
        Vector2 screenMin;
        Vector2 screenMax;

        /// <summary>
        /// w’è”ÍˆÍŠO‚Öo‚Ä‚¢‚½ê‡Œü‚©‚¤•ûŒü‚ğ•Ï‚¦Ø‚è•Ô‚·
        /// </summary>
        /// <param name="number">’e‚Ì”z—ñ‚Ì”š</param>
        void OutRenge (int number);
        /// <summary>
        /// Œ»İ‚¢‚éêŠ‚É‰‚¶‚ÄŒü‚©‚¤•ûŒü‚ğ—^‚¦‚é
        /// </summary>
        /// <param name="number">’e‚Ì”z—ñ‚Ì”š</param>
        void VectorSet(int number);
    public:
        Frame();
        //‰ŠúˆÊ’u‚Ìİ’è
        void PositionSet();
        void Update();
    };
}
