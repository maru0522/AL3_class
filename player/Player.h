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
	// 初期化
	void Initialize(Model* model,uint32_t textureHandle);
	// 移動
	void Move();
	// 軸回転
	void Rotate();
	// 攻撃
	void Attack();
	// 更新
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	// 弾
	PlayerBullet* bullet_ = nullptr;
};