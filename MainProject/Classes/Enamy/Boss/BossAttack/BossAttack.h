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
        /// �ő吔���猻�݂������ڂ��ɉ����Č������������v�Z���Ԃ�
        /// </summary>
        /// <param name="shotVectorY">  ���������������(��::-1,��::1)</param>
        /// <param name="shotMax">      �e�̍ő吔</param>
        ///  <param name="bulletNumber">�����ڂ�</param>
        /// <returns>����������</returns>
        Vector2 ShotVectorSet(float shotVectorY,float shotMax,float bulletNumber);

    public:
        /// <summary>
        /// �w������Ɂ���ɔ��ł���
        /// </summary>
        /// <param name="moveVector">�����킹��������</param>
        void ShotMove(float moveVector);

        /// <summary>
        /// �w��ʒu�Ɉړ�
        /// </summary>
        /// <param name="position">�e�����˂����ʒu</param>
        void ShotPreparation(Vector2 position);

    };

    class AimShot :public BossManager
    {
    private:
        CF::Timer shotTimer;

        Vector2 shotVector[AIMSHOT_BULLET_MAX];//���@�_���̂��߂̕����ۊǗp�ϐ�

        bool isShotMove; //���˂̊Ǘ�
        bool isPlaceMove;//�e�̒�ʒu�܂ł̈ړ��Ǘ�

        /// <summary>
        /// �e�������ڂ��ɉ����Ĉړ���̈ʒu���v�Z���Ԃ�
        /// </summary>
        /// <param name="bulletNumber">�����ڂ�</param>
        /// <returns>�ʒu</returns>
        Vector2 SetAttackReserve(int bulletNumber);
    public:
        AimShot():
        isShotMove(false),isPlaceMove(false)
        {}

        /// <summary>
        /// ����������
        /// </summary>
        void ShotReserve();

        /// <summary>
        /// �����Ɉړ������@�_��
        /// </summary>
        /// <param name="startPosition">�e�̍U�����n�܂�</param>
        /// <param name="targetPosition">�_���ʒu</param>
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
        Vector2 xRange;
        Vector2 yRange;

        bool isVectorSwitch;

        /// <summary>
        /// �w��͈͊O�֏o�Ă����ꍇ������������ς��؂�Ԃ�
        /// </summary>
        /// <param name="number">�e�̔z��̐���</param>
        void OutRange (int number);
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
        void SetIsVectorSwitch(bool isFlag) { isVectorSwitch = isFlag; }

        Vector2 GetPosition  (int bulletNumber) { return position[bulletNumber]; }
        Vector2 GetMoveVector(int bulletNumber) { return move[bulletNumber];     }
        float   GetAngle     (int bulletNumber) { return angle[bulletNumber];    }
    };
}
