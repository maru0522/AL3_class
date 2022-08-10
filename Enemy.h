#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "input.h"
#include "DebugText.h"

enum class Phase {
	Approach,	// 接近する
	Leave,		// 離脱する
};

class Enemy;

class EnemyState
{
public:
	// 純粋仮想関数
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
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	DebugText* debugText_ = nullptr;

	// フェーズ
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