﻿#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
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

	// X, Y, Z方向のスケーリングを設定
	worldTransform_.scale_ = { 2,2,2 };
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { 5.0f, 0.0f, 0.0f, 0.0f,//x
				 0.0f, 1.0f, 0.0f, 0.0f,//y
				 0.0f, 0.0f, 1.0f, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	worldTransform_.matWorld_ = {1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1 };
	worldTransform_.matWorld_.operator*=(matScale);
	worldTransform_.TransferMatrix();
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
	// model_->Draw(worldTransform_,viewProjection_,textureHandle_);
	////モデルと連動させるカメラの描画
	// model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//ラインの描画
	//拡大された立方体の描画

	/*for (int i = 0; i < _countof(edgeList); i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			vertex[edgeList[i][0]], vertex[edgeList[i][1]], WHITE);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			scaleVertex[edgeList[i][0]], scaleVertex[edgeList[i][1]], BLUE);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			rotaXVertex[edgeList[i][0]], rotaXVertex[edgeList[i][1]], RED);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			rotaYVertex[edgeList[i][0]], rotaYVertex[edgeList[i][1]], GREEN);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			rotaZVertex[edgeList[i][0]], rotaZVertex[edgeList[i][1]], BLACK);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			translationVertex[edgeList[i][0]], translationVertex[edgeList[i][1]], PINK);
	}*/

	model_->Draw(worldTransform_,debugCamera_->GetViewProjection(), textureHandle_);

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