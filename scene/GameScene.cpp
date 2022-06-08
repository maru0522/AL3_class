#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {


	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engin(seed_gen());
	// 乱数範囲の指定
	std::uniform_real_distribution<float> dist(0, 100);
	// 乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, 2 * PI);
	// 乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Task1_2Resources/mario.jpg");

	// 範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X, Y, Z方向のスケーリングを設定
		worldTransform.scale_ = { 1,1,1 };
		// X, Y, Z 軸周りの回転角を設定
		worldTransform.rotation_ = { rotDist(engin),rotDist(engin),rotDist(engin) };
		// X, Y, Z 軸周りの平行移動を設定
		worldTransform.translation_ = { posDist(engin),posDist(engin),posDist(engin) };

		// スケーリング行列を宣言
		Matrix4 matScale;
		// スケーリング倍率を行列に設定する
		matScale = { worldTransform.scale_.x, 0.0f, 0.0f, 0.0f,//x
					 0.0f, worldTransform.scale_.y, 0.0f, 0.0f,//y
					 0.0f, 0.0f, worldTransform.scale_.z, 0.0f,//z
					 0.0f, 0.0f, 0.0f, 1.0f };
		// 合成用回転行列を宣言
		Matrix4 matRot;
		// 各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		// Z軸回転行列の各要素を設定する
		matRotZ = { cos(worldTransform.rotation_.z), sin(worldTransform.rotation_.z), 0.0f, 0.0f,	//x
					-sin(worldTransform.rotation_.z), cos(worldTransform.rotation_.z), 0.0f, 0.0f,//y
					0.0f,0.0f, 1.0, 0.0f,															//z
					0.0f, 0.0f, 0.0f, 1.0f };
		// X軸回転行列の各要素を設定する
		matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,															//x
					0.0f, cos(worldTransform.rotation_.x), sin(worldTransform.rotation_.x), 0.0f,	//y
					0.0f, -sin(worldTransform.rotation_.x), cos(worldTransform.rotation_.x), 0.0f,//z
					0.0f, 0.0f, 0.0f, 1.0f };
		// Y軸回転行列の各要素を設定する
		matRotY = { cos(worldTransform.rotation_.y), 0.0f, -sin(worldTransform.rotation_.y), 0.0f,//x
					0.0f, 1.0f, 0.0f, 0.0f,	//y
					sin(worldTransform.rotation_.y),0.0f, cos(worldTransform.rotation_.y), 0.0f,//z
					0.0f, 0.0f, 0.0f, 1.0f };

		// 各軸の回転行列を合成

		matRot = matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;

		// 合成用回転行列を宣言
		Matrix4 matTrans;
		//matTrans = MathUtility::Matrix4Identity();

		// 移動量を行列に設定する。
		matTrans = { 1,0,0,0,
					 0,1,0,0,
					 0,0,1,0,
					 worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z,1 };

		// 行列の合成
		worldTransform.matWorld_ = MathUtility::Matrix4Identity();
		worldTransform.matWorld_ *= matScale;
		worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matTrans;

		// 行列の転送
		worldTransform.TransferMatrix();
	}
	//viewProjection_.eye = { 0,0,-10 };

	viewProjection_.target = { 10,0,0 };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();

	// 視点移動処理
	{
		// 視点の移動ベクトル
		Vector3 move = { 0,0,0 };

		// 視点の移動速度
		const float kEyeSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move = { 0,0,kEyeSpeed };
		}
		if (input_->PushKey(DIK_S)) {
			move = { 0,0,-kEyeSpeed };
		}

		// 視点移動（ベクトルの加算）
		viewProjection_.eye += move;

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 50);
		debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	// 注視点移動処理
	{
		// 注視点移動ベクトル
		Vector3 move = { 0,0,0 };

		// 注視点の移動速度
		const float kTargetSpeed = 0.2f;

		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kTargetSpeed,0,0 };
		}
		if (input_->PushKey(DIK_RIGHT)) {
			move = { 0,0,kTargetSpeed };
		}
	}
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
	// model_->Draw(worldTransform_,viewProjection_,textureHandle_);
	////モデルと連動させるカメラの描画
	// model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 範囲forで全てのワールドトランスフォームを順に処理する
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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