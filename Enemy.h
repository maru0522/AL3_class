#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"

enum class Phase {
	Approach,	// �ڋ߂���
	Leave,		// ���E����
};

class Enemy
{
private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	DebugText* debugText_ = nullptr;

	// �t�F�[�Y
	Phase phase_ = Phase::Approach;

public:
	void Initialize(Model* model, uint32_t textureHandle);
    void Update();
    void Draw(ViewProjection viewProjection);
};

