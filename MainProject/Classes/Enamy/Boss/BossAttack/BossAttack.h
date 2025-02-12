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
        bool    isShot    [NORMAL_SHOT_BULLET_MAX];//�e�̔��ˊǗ�
        Vector2 moveVector[NORMAL_SHOT_BULLET_MAX];//�U������
        int     normalOneShot = NORMAL_ONESHOT_MAX;
        /// <summary>
        /// �ő吔���猻�݂������ڂ��ɉ����Č�����������Ԃ�
        /// ���~��ɓW�J����
        /// </summary>
        /// <param name="shotVectorY">���������������(��::-1,��::1)</param>
        /// <param name="oneShotBulletMax">�e�̍ő吔</param>
        /// <param name="bulletNumber">���݉����ڂ�</param>
        /// <returns>����������</returns>
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
        //���˂��Ǘ�����ϐ�
        bool isShotMove[AIMSHOT_COUNT];
        bool isPlaceMove[AIMSHOT_COUNT];//�e�̒�ʒu�܂ł̈ړ��Ǘ�


        Vector2 SetAttackReserve(int attackCount,int bulletNumber);

    public:
        AimShot();
        /// <summary>
        /// ����������
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
        //�r���Œǔ������is��
        bool isMove;
        bool isPositionUpdate;
        //�e�̑傫��
        int volume;
    public:
        Induction();

        /// <summary>
        /// �ǔ��e�̋N��
        /// </summary>
        /// <param name="startPosition">�ǔ��̎n�܂�|�C���g</param>
        void InductionStart(Vector2 startPosition);

        /// <summary>
        /// �ǔ�
        /// </summary>
        /// <param name="playerPosition">�ǔ��Ώ�</param>
        void Update(Vector2 playerPosition);

    };

    class Frame :public BossManager
    {
    private:
        Vector2 move[FRAME_BULLET_MAX];
        Vector2 screenMin;
        Vector2 screenMax;

        /// <summary>
        /// �w��͈͊O�֏o�Ă����ꍇ������������ς��؂�Ԃ�
        /// </summary>
        /// <param name="number">�e�̔z��̐���</param>
        void OutRenge (int number);
        /// <summary>
        /// ���݂���ꏊ�ɉ����Č�����������^����
        /// </summary>
        /// <param name="number">�e�̔z��̐���</param>
        void VectorSet(int number);
    public:
        Frame();
        //�����ʒu�̐ݒ�
        void PositionSet();
        void Update();
    };
}
