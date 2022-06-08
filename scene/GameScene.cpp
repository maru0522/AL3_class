#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

#define RADIANS(deg) (deg * PI / 180)
#define DEGREES(radian) (180 / PI * radian)

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

void GameScene::SetFovAngleY()
{
	// FOV変更処理
		// 上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(PI, viewProjection_.fovAngleY);
		}
		// 下キーで視野角が狭まる
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(0.01f, viewProjection_.fovAngleY);
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", DEGREES(viewProjection_.fovAngleY));
}

void GameScene::SetNearZ()
{
	// クリップ距離変更処理
		// 上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.5f;
		}
		if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.5f;
		}

		// 行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Task1_2Resources/mario.jpg");

	// 親（0番）
	worldTransforms_[0].Initialize();
	// 子（1番）
	worldTransforms_[1].Initialize();
	worldTransforms_[1].translation_ = { 0.0f,4.5f,0.0f };
	worldTransforms_[1].parent_ = &worldTransforms_[0];


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

void GameScene::UpdateMatrix(int idx)
{
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { worldTransforms_[idx].scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, worldTransforms_[idx].scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, worldTransforms_[idx].scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	// 合成用回転行列を宣言
	Matrix4 matRot;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ = { cos(worldTransforms_[idx].rotation_.z), sin(worldTransforms_[idx].rotation_.z), 0.0f, 0.0f,	//x
				-sin(worldTransforms_[idx].rotation_.z), cos(worldTransforms_[idx].rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,																//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列の各要素を設定する
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,																//x
				0.0f, cos(worldTransforms_[idx].rotation_.x), sin(worldTransforms_[idx].rotation_.x), 0.0f,	//y
				0.0f, -sin(worldTransforms_[idx].rotation_.x), cos(worldTransforms_[idx].rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y軸回転行列の各要素を設定する
	matRotY = { cos(worldTransforms_[idx].rotation_.y), 0.0f, -sin(worldTransforms_[idx].rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,	//y
				sin(worldTransforms_[idx].rotation_.y),0.0f, cos(worldTransforms_[idx].rotation_.y), 0.0f,	//z
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
				 worldTransforms_[idx].translation_.x,worldTransforms_[idx].translation_.y,worldTransforms_[idx].translation_.z,1 };

	// 行列の合成
	worldTransforms_[idx].matWorld_ = MathUtility::Matrix4Identity();
	worldTransforms_[idx].matWorld_ *= matScale;
	worldTransforms_[idx].matWorld_ *= matRot;
	worldTransforms_[idx].matWorld_ *= matTrans;

	// 行列の転送
	worldTransforms_[idx].TransferMatrix();
}
void GameScene::UpdateMatrixChild(int idx)
{
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { worldTransforms_[idx].scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, worldTransforms_[idx].scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, worldTransforms_[idx].scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	// 合成用回転行列を宣言
	Matrix4 matRot;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ = { cos(worldTransforms_[idx].rotation_.z), sin(worldTransforms_[idx].rotation_.z), 0.0f, 0.0f,	//x
				-sin(worldTransforms_[idx].rotation_.z), cos(worldTransforms_[idx].rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,																//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列の各要素を設定する
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,																//x
				0.0f, cos(worldTransforms_[idx].rotation_.x), sin(worldTransforms_[idx].rotation_.x), 0.0f,	//y
				0.0f, -sin(worldTransforms_[idx].rotation_.x), cos(worldTransforms_[idx].rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y軸回転行列の各要素を設定する
	matRotY = { cos(worldTransforms_[idx].rotation_.y), 0.0f, -sin(worldTransforms_[idx].rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,	//y
				sin(worldTransforms_[idx].rotation_.y),0.0f, cos(worldTransforms_[idx].rotation_.y), 0.0f,	//z
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
				 worldTransforms_[idx].translation_.x,worldTransforms_[idx].translation_.y,worldTransforms_[idx].translation_.z,1 };

	// 行列の合成
	worldTransforms_[idx].matWorld_ = MathUtility::Matrix4Identity();
	worldTransforms_[idx].matWorld_ *= matScale;
	worldTransforms_[idx].matWorld_ *= matRot;
	worldTransforms_[idx].matWorld_ *= matTrans;
	worldTransforms_[idx].matWorld_ *= worldTransforms_[0].matWorld_;

	// 行列の転送
	worldTransforms_[idx].TransferMatrix();
}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();

	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		// キャラクターの移動速度
		const float kCharacterSpeed = 0.2f;

		if (input_->PushKey(DIK_A)) {
			move = { -kCharacterSpeed,0,0 };
		}
		if (input_->PushKey(DIK_D)) {
			move = { kCharacterSpeed,0,0 };
		}

		worldTransforms_[0].translation_ += move;
		UpdateMatrix(0);
		
		debugText_->SetPos(50, 150);
		debugText_->Printf("worldTransforms_[%d].translation:(%f,%f,%f)", 0, 
						   worldTransforms_[0].translation_.x,
						   worldTransforms_[0].translation_.y,
						   worldTransforms_[0].translation_.z);

		// 子の更新
		{
			UpdateMatrixChild(1);
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

	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);
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