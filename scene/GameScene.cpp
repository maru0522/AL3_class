#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

const float PI = 3.14159265358979323846f;

#define RADIANS(deg) (deg * PI / 180)

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::affin()
{
	// X, Y, Z方向のスケーリングを設定
	worldTransform_.scale_ = { 5,2,2 };
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { worldTransform_.scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, worldTransform_.scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, worldTransform_.scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	// X, Y, Z 軸周りの回転角を設定
	worldTransform_.rotation_ = { RADIANS(45),RADIANS(45),RADIANS(45) };
	// 合成用回転行列を宣言
	Matrix4 matRot;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ = { cos(worldTransform_.rotation_.z), sin(worldTransform_.rotation_.z), 0.0f, 0.0f,	//x
				-sin(worldTransform_.rotation_.z), cos(worldTransform_.rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,															//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列の各要素を設定する
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,															//x
				0.0f, cos(worldTransform_.rotation_.x), sin(worldTransform_.rotation_.x), 0.0f,	//y
				0.0f, -sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y軸回転行列の各要素を設定する
	matRotY = { cos(worldTransform_.rotation_.y), 0.0f, -sin(worldTransform_.rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,	//y
				sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };

	// 各軸の回転行列を合成

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// X, Y, Z 軸周りの平行移動を設定
	worldTransform_.translation_ = { 10,10,10 };
	// 合成用回転行列を宣言
	Matrix4 matTrans;
	//matTrans = MathUtility::Matrix4Identity();

	// 移動量を行列に設定する。
	matTrans = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1 };

	// 行列の合成
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	// 行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Task1_2Resources/mario.jpg");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

#pragma region 各変換
	//// スケール
	//{
	//	// X, Y, Z方向のスケーリングを設定
	//	worldTransform_.scale_ = { 5,1,1 };
	//	// スケーリング行列を宣言
	//	Matrix4 matScale;
	//	// スケーリング倍率を行列に設定する
	//	matScale = { worldTransform_.scale_.x, 0.0f, 0.0f, 0.0f,//x
	//				 0.0f, worldTransform_.scale_.y, 0.0f, 0.0f,//y
	//				 0.0f, 0.0f, worldTransform_.scale_.z, 0.0f,//z
	//				 0.0f, 0.0f, 0.0f, 1.0f };

	//	worldTransform_.matWorld_ = { 1,0,0,0,
	//								  0,1,0,0,
	//								  0,0,1,0,
	//								  0,0,0,1 };
	//	worldTransform_.matWorld_.operator*=(matScale);
	//	worldTransform_.TransferMatrix();
	//}

	//// 回転
	//{
	//	// 回転の合成
	//	{
	//		// X, Y, Z 軸周りの回転角を設定
	//		worldTransform_.rotation_ = { RADIANS(45),RADIANS(45),RADIANS(45) };
	//		// 合成用回転行列を宣言
	//		Matrix4 matRot;
	//		// 各軸用回転行列を宣言
	//		Matrix4 matRotX, matRotY, matRotZ;

	//		// Z軸回転行列の各要素を設定する
	//		matRotZ = { cos(worldTransform_.rotation_.z), sin(worldTransform_.rotation_.z), 0.0f, 0.0f,	//x
	//					-sin(worldTransform_.rotation_.z), cos(worldTransform_.rotation_.z), 0.0f, 0.0f,//y
	//					0.0f,0.0f, 1.0, 0.0f,															//z
	//					0.0f, 0.0f, 0.0f, 1.0f };
	//		// X軸回転行列の各要素を設定する
	//		matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,															//x
	//					0.0f, cos(worldTransform_.rotation_.x), sin(worldTransform_.rotation_.x), 0.0f,	//y
	//					0.0f, -sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x), 0.0f,//z
	//					0.0f, 0.0f, 0.0f, 1.0f };
	//		// Y軸回転行列の各要素を設定する
	//		matRotY = { cos(worldTransform_.rotation_.y), 0.0f, -sin(worldTransform_.rotation_.y), 0.0f,//x
	//					0.0f, 1.0f, 0.0f, 0.0f,	//y
	//					sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y), 0.0f,//z
	//					0.0f, 0.0f, 0.0f, 1.0f };

	//		// 各軸の回転行列を合成

	//		matRot = matRotZ;
	//		matRot *= matRotX;
	//		matRot *= matRotY;

	//		worldTransform_.matWorld_ = { 1,0,0,0,
	//									  0,1,0,0,
	//									  0,0,1,0,
	//									  0,0,0,1 };

	//		worldTransform_.matWorld_.operator*=(matRot);
	//		worldTransform_.TransferMatrix();
	//	}
	//}

	//// 平行移動
	//{
	//		// X, Y, Z 軸周りの平行移動を設定
	//		worldTransform_.translation_ = { 0,0,0 };
	//		// 合成用回転行列を宣言
	//		Matrix4 matTrans;
	//		//matTrans = MathUtility::Matrix4Identity();

	//		// 移動量を行列に設定する。
	//		matTrans = { 1,0,0,0,
	//					 0,1,0,0,
	//					 0,0,1,0,
	//					 worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1 };

	//		worldTransform_.matWorld_ = { 1,0,0,0,
	//									  0,1,0,0,
	//									  0,0,1,0,
	//									  0,0,0,1 };

	//		worldTransform_.matWorld_.operator*=(matTrans);
	//		worldTransform_.TransferMatrix();
	//}
#pragma endregion

	// 関数化
	affin();
}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	////3Dモデルの描画


	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}