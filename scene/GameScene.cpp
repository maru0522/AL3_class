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

void GameScene::Afiin()
{
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

		worldTransform.Affin();
	}
}

void GameScene::EyeMove()
{
	// 視点移動処理
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

void GameScene::TargetMove()
{
	// 注視点移動処理
		// 注視点移動ベクトル
	Vector3 move = { 0,0,0 };

	// 注視点の移動速度
	const float kTargetSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kTargetSpeed,0,0 };
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move = { kTargetSpeed,0,0, };
	}

	// 注視点移動（ベクトルの加算）
	viewProjection_.target += move;

	// 行列の再計算
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 70);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
}

void GameScene::UpRot()
{
	// 上方向回転処理
		// 上方向の回転速度[ラジアン/frame]
	const float kUpRotSpeed = 0.05f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		// 2PIを超えたら0に戻す
		viewAngle = fmodf(viewAngle, PI * 2.0f);
	}

	// 上方向ベクトルを計算（半径1の円周上の座標）
	viewProjection_.up = { cos(viewAngle),sin(viewAngle),0.0f };

	// 行列の再計算
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 90);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
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

		Matrix4 matScale_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixScale(matScale_);

		Matrix4 matRotZ_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixRotateZ(matRotZ_);
		Matrix4 matRotX_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixRotateX(matRotX_);
		Matrix4 matRotY_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixRotateY(matRotY_);

		Matrix4 matRot_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixRotate(matRot_, matRotX_, matRotY_, matRotZ_);
		
		Matrix4 matTrans_ = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixTranslate(matTrans_);

		Matrix4 matWorldTransform = MathUtility::Matrix4Identity();
		worldTransform.SetMatrixWorldTransform(matWorldTransform, matScale_, matRot_, matTrans_);

		worldTransform.TransferMatrix();
	}

	//viewProjection_.eye = { 0,0,-10 };

	viewProjection_.target = { 10,0,0 };

	// カメラ上方向ベクトルを設定（右上45度指定）
	viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),0.0f };


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

	EyeMove();
	TargetMove();
	UpRot();
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