#pragma once
#include "..\BossManager\BossManager.h"
using namespace DirectX;
using namespace SimpleMath;
namespace BossAttack
{
    class Normal:public BossManager
    {
    private:
        /// <summary>
        /// 最大数から現在が何発目かに応じて向かう方向を計算し返す
        /// </summary>
        /// <param name="shotVectorY">下方向か上方向か(上::-1,下::1)</param>
        /// <param name="oneShotBulletMax">弾の最大数</param>
        ///  <param name="oneShotBulletMax">何発目か</param>
        /// <returns>向かう方向</returns>
        Vector2 ShotVectorSet(float shotVectorY,float shotMax,float bulletNumber);

    public:
        /// <summary>
        /// 指定方向に▽状に飛んでいく
        /// </summary>
        /// <param name="moveVector">向かわせたい方向</param>
        void ShotMove(float moveVector);
        void ShotPreparation(Vector2 bossPosition);
    };

    class AimShot :public BossManager
    {
    private:
        CF::Timer shotTimer;
        Vector2 shotVector[AIMSHOT_BULLET_MAX];

        //発射を管理する変数
        bool isShotMove;
        bool isPlaceMove;//弾の定位置までの移動管理

        Vector2 SetAttackReserve(int bulletNumber);

    public:
        AimShot();

        bool GetIsPlaceMove() { return isPlaceMove; }

        /// <summary>
        /// 初期化処理
        /// </summary>
        void ShotReserve();
        void Update(Vector2 BossPosition,Vector2 playerPosition);
    };

    class Induction :public BossManager
    {
    private:
        CF::Timer largeTimer;
        CF::Timer attackCoolTimer;

        SimpleMath::Vector2 moveVector;
        SimpleMath::Vector2 oldPlayerPosition;
        //途中で追尾をやめisる
        bool isMove;
        bool isPositionUpdate;
        //弾の大きさ
        int volume;
    public:
        Induction();

        int GetVolume() { return volume; }

        /// <summary>
        /// 追尾弾の起動
        /// </summary>
        /// <param name="startPosition">追尾の始まるポイント</param>
        void InductionStart(Vector2 startPosition);

        /// <summary>
        /// 追尾
        /// </summary>
        /// <param name="playerPosition">追尾対象</param>
        void Update(Vector2 playerPosition);

    };

    class Frame :public BossManager
    {
    private:
        Vector2 move[FRAME_BULLET_MAX];
        Vector2 screenMin;
        Vector2 screenMax;

        /// <summary>
        /// 指定範囲外へ出ていた場合向かう方向を変え切り返す
        /// </summary>
        /// <param name="number">弾の配列の数字</param>
        void OutRenge (int number);
        /// <summary>
        /// 現在いる場所に応じて向かう方向を与える
        /// </summary>
        /// <param name="number">弾の配列の数字</param>
        void VectorSet(int number);
    public:
        Frame();
        //初期位置の設定
        void PositionSet();
        void Update();
    };
}
