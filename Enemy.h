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

class Enemy;

class EnemyState
{
public:
	// �������z�֐�
	virtual void Up(Enemy* pEnemy) = 0;
	virtual void Down(Enemy* pEnemy) = 0;
	virtual void ShowState() = 0;
};

class EnemyApproach : public EnemyState
{
public:
	void Up(Enemy* pEnemy);
	void Down(Enemy* pEnemy);
	void ShowState();
};

class EnemyLeave : public EnemyState
{
public:
	void Up(Enemy* pEnemy);
	void Down(Enemy* pEnemy);
	void ShowState();
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
	Enemy();
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);
    void Update();
    void Draw(ViewProjection viewProjection);

	void Up();
	void Down();

	void ChangeState(EnemyState* newState);
	void ShowNowState();

private:
	EnemyState* state;
};