#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"
#include "playerBullet/PlayerBullet.h"

class Player
{
public:
	// ������
	void Initialize(Model* model,uint32_t textureHandle);
	// �ړ�
	void Move();
	// ����]
	void Rotate();
	// �U��
	void Attack();
	// �X�V
	void Update();
	// �`��
	void Draw(ViewProjection viewProjection);

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// �e
	PlayerBullet* bullet_ = nullptr;
};