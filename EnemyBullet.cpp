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
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	velocity_ = velocity;
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

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}


Vector3 EnemyBullet::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
