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
	void Initialize(Model* model, uint32_t textureHandle);
    void Update();
    void Draw(ViewProjection viewProjection);
};

