#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// Null�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("Task1_2Resources/enemyBullet.png");

	worldTransform_.Initialize();

	// Z�����ɐL�т��`��
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// Y������̊p�x(��y)
	worldTransform_.rotation_.y = std::atan2(velocity_.z, velocity_.x);
}


void EnemyBullet::Update()
{
	// ���W���ړ�������
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	// ���Ԍo�߂�death
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}


void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
