#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"

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

public:
	void Initialize(Model* model, uint32_t textureHandle);
    void Update();
    void Draw(ViewProjection viewProjection);
};

