#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "EnemyBullet.h"

// ���@�N���X�̑O���錾
class Player;

enum class Phase {
	Approach,	// �ڋ߂���
	Leave,		// ���E����
};

class Enemy
{
private:
	// ���ˊԊu
	static const int kFireInterval = 60;

	// ���˃^�C�}
	int32_t fireTimer = 0;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	DebugText* debugText_ = nullptr;

	// �t�F�[�Y
	Phase phase_ = Phase::Approach;

	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// ���L����
	Player* player_ = nullptr;

public:
	void Initialize(Model* model, uint32_t textureHandle);
	void Fire();
	void PhaseInitApproach();
	void PhaseApproach();
	void PhaseLeave();
    void Update();
    void Draw(ViewProjection viewProjection);
	void SetPlayer(Player* player) { player_ = player; }
	// ���[���h���W���擾
	Vector3 GetWorldPosition();
};

