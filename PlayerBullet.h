#pragma once
#include "Model.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool isDead() const { return isDead_; }

	// �Փ˂����o������Ăт������R�[���o�b�N�֐�
	void OnCollision();

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	// ���x
	Vector3 velocity_;

	// ���� <frame>
	static const int32_t kLifeTime = 60 * 5;
	// �f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	// �f�X�t���O
	bool isDead_ = false;
};