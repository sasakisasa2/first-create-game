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
        /// <param name="shotVectorY">  下方向か上方向か(上::-1,下::1)</param>
        /// <param name="shotMax">      弾の最大数</param>
        ///  <param name="bulletNumber">何発目か</param>
        /// <returns>向かう方向</returns>
        Vector2 ShotVectorSet(float shotVectorY,float shotMax,float bulletNumber);

    public:
        /// <summary>
        /// 指定方向に▽状に飛んでいく
        /// </summary>
        /// <param name="moveVector">向かわせたい方向</param>
        void ShotMove(float moveVector);

        /// <summary>
        /// 指定位置に移動
        /// </summary>
        /// <param name="position">弾が発射される位置</param>
        void ShotPreparation(Vector2 position);

    };

    class AimShot :public BossManager
    {
    private:
        CF::Timer shotTimer;

        Vector2 shotVector[AIMSHOT_BULLET_MAX];//自機狙いのための方向保管用変数

        bool isShotMove; //発射の管理
        bool isPlaceMove;//弾の定位置までの移動管理

        /// <summary>
        /// 弾が何発目かに応じて移動先の位置を計算し返す
        /// </summary>
        /// <param name="bulletNumber">何発目か</param>
        /// <returns>位置</returns>
        Vector2 SetAttackReserve(int bulletNumber);
    public:
        AimShot():
        isShotMove(false),isPlaceMove(false)
        {}

        /// <summary>
        /// 初期化処理
        /// </summary>
        void ShotReserve();

        /// <summary>
        /// 横一列に移動し自機狙い
        /// </summary>
        /// <param name="startPosition">弾の攻撃が始まる</param>
        /// <param name="targetPosition">狙う位置</param>
        void Update(Vector2 startPosition,Vector2 targetPosition);
    };

    class Induction :public BossManager
    {
    private:
        CF::Timer largeTimer;
        CF::Timer attackCoolTimer;

        SimpleMath::Vector2 moveVector;
        SimpleMath::Vector2 oldPlayerPosition;
        bool isMove;
        bool isPositionUpdate;
        //弾の大きさ
        int volume;
    public:
        Induction();

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
        Vector2 xRange;
        Vector2 yRange;

        bool isVectorSwitch;

        /// <summary>
        /// 指定範囲外へ出ていた場合向かう方向を変え切り返す
        /// </summary>
        /// <param name="number">弾の配列の数字</param>
        void OutRange (int number);
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
        void SetIsVectorSwitch(bool isFlag) { isVectorSwitch = isFlag; }

        Vector2 GetPosition  (int bulletNumber) { return position[bulletNumber]; }
        Vector2 GetMoveVector(int bulletNumber) { return move[bulletNumber];     }
        float   GetAngle     (int bulletNumber) { return angle[bulletNumber];    }
    };
}
