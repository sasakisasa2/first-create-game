#include "BossManager.h"
BossManager::BossManager() :
	position(FRAME_BULLET_MAX, Vector2(640.0f,100.0f)),
	color   (FRAME_BULLET_MAX, Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
	angle   (FRAME_BULLET_MAX, 0.0f),
	effect  (SpriteEffects_None)
{
	bossSpriteSize           = Vector2(200.0f,200.0f);
	moveSpeed                = 500.0f;
	//�ʏ�U��
	normalSpriteSize         = Vector2(50.0f,50.0f);
	normalShotSpeed          = 300.0f;
	normalShotPlace          = 0.3f;
	//���@�_���U��
	aimShotSpriteSize       = Vector2(50.0f,50.0f);
	aimShotPlaceMin         = 200.0f;
	aimShotShotPlace        = ((float)DXTK->Screen.Width - (aimShotPlaceMin*2.0f)) / (float)AIMSHOT_BULLET_MAX;
	aimShotShotHyde         = 100.0f;
	aimShotSettingSpeed     = 1000.0f;
	aimShotShotSpeed        = 1000.0f;
	toShotTime              = 2.0f;
	//�ǔ��U��
	inductionSpriteSize      = Vector2(100.0f,100.0f);
	inductionMoveSpeed       = 300.0f;
	inductionLastAttackSpeed = 1000.0f;
	volumeLargeTime	         = 2.0f;
	maxVolume                = 3;
	//�t���[���U��
	frameSize                = Vector2(150.0f,150.0f);
	frameMoveSpeed           = 1000.0f;
	frameQuarter             = (float)FRAME_BULLET_MAX / 4.0f;
	frameEighth	             = (float)FRAME_BULLET_MAX / 8.0f;
}
void BossManager::Load(DirectXTK::Sprite& preSprite)
{
	sprite = preSprite;
}

void BossManager::Render(DirectX::SpriteBatch* SpriteBatch,int count) 
{
	SpriteBatch->Draw(
		sprite.handle, sprite.size, position[count],
		nullptr,//�摜�̂ǂ��ɕ\�����邩
		color[count],//�F�̊Ԉ�����(�J���[�}�X�N)
		angle[count] * Mathf::Deg2Rad,
		Vector2(sprite.size.x / 2.0f, sprite.size.y / 2.0f),//��]�̊p�x�Ǝ�
		1.0f,
		effect
	);
}