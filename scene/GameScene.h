#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "math.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t reticleHandle_ = 0;

	//
	Sprite* sprite_ = nullptr;


	WorldTransform worldTransforms_[100];

	ViewProjection viewProjection_;

	float PI = 3.141592f;

	// カメラ上方向の角度
	float viewAngle = 0.0f;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

public:
	// パーツID
	enum PartId {
		kRoot,		// 大元
		kSpine,		// 脊椎
		kChest,		// 胸
		kHead,		// 頭
		kArmL,		// 左腕
		kArmR,		// 右腕
		kHip,		// 尻
		kLegL,		// 左足
		kLegR,		// 右足

		kNumPartId
	};

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 行列の更新
	void UpdateMatrix(WorldTransform& worldtransform);

	// 度数法(degree)から弧度法(radian)
	float Deg2Rad(int degree) { return (degree * PI / 180); }
	// 弧度法(radian)から度数法(degree)
	float Rad2Deg(int radian) { return (180 / PI * radian); }
};