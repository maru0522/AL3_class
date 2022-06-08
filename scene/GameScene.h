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

public:
	// パーツID
	enum PartId {
		Root,		// 大元
		Spine,	// 脊椎
		Chest,	// 胸
		Head,		// 頭
		ArmL,		// 左腕
		ArmR,		// 右腕
		Hip,		// 尻
		LegL,		// 左足
		LegR,		// 右足
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

	void Afiin();
	void EyeMove();
	void TargetMove();
	void UpRot();
	void SetFovAngleY();
	void SetNearZ();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void UpdateMatrix(int idx);
	void UpdateMatrixChild(int idx);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

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
};