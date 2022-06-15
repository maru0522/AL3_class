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

    dxCommon_ = DirectXCommon::GetInstance();
    input_ = Input::GetInstance();
    audio_ = Audio::GetInstance();
    debugText_ = DebugText::GetInstance();
    model_ = Model::Create();
    //ファイル名を指定してテクスチャを読み込む
    textureHandle_ = TextureManager::Load("Task1_2Resources/mario.jpg");

    // 大元
    worldTransforms_[PartId::kRoot].Initialize();
    // 脊椎
    worldTransforms_[PartId::kSpine].Initialize();
    worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[kRoot];
    worldTransforms_[PartId::kSpine].translation_ = { 0.0f,4.5f,0.0f };

    // 上半身
    // 胸
    worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };
    worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
    worldTransforms_[PartId::kChest].Initialize();
    // 頭
    worldTransforms_[PartId::kHead].translation_ = { 0, 3,0 };
    worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
    worldTransforms_[PartId::kHead].Initialize();
    // 左腕
    worldTransforms_[PartId::kArmL].translation_ = { -3,0,0 };
    worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
    worldTransforms_[PartId::kArmL].Initialize();
    // 右腕
    worldTransforms_[PartId::kArmR].translation_ = { 3,0,0 };
    worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
    worldTransforms_[PartId::kArmR].Initialize();
    // 下半身
    // 尻
    worldTransforms_[PartId::kHip].translation_ = { 0,-3,0 };
    worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
    worldTransforms_[PartId::kHip].Initialize();
    // 左足
    worldTransforms_[PartId::kLegL].translation_ = { -3,-3,0 };
    worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
    worldTransforms_[PartId::kLegL].Initialize();
    // 右足
    worldTransforms_[PartId::kLegR].translation_ = { 3,-3,0 };
    worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
    worldTransforms_[PartId::kLegR].Initialize();



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

        worldTransforms_[PartId::kRoot].translation_ += move;

        debugText_->SetPos(50, 150);
        debugText_->Printf("worldTransforms_[%d].translation:(%f,%f,%f)", 0,
                           worldTransforms_[0].translation_.x,
                           worldTransforms_[0].translation_.y,
                           worldTransforms_[0].translation_.z);

        for (int i = 0; i < kNumPartId; i++) {
            Transform(worldTransforms_[i]);
        }
    }

    // 上半身回転処理
    {
        // 押した方向で移動ベクトルを変更
        if (input_->PushKey(DIK_U)) {
            worldTransforms_[PartId::kChest].rotation_.y -= 0.05;
        }
        if (input_->PushKey(DIK_I)) {
            worldTransforms_[PartId::kChest].rotation_.y += 0.05;
        }
    }

    // 下半身回転処理
    {
        // 押した方向で移動ベクトルを変更
        if (input_->PushKey(DIK_J)) {
            worldTransforms_[PartId::kHip].rotation_.y -= 0.05;
        }
        if (input_->PushKey(DIK_K)) {
            worldTransforms_[PartId::kHip].rotation_.y += 0.05;
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

    for (int i = 2; i < kNumPartId; i++) {
        model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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


void GameScene::Transform(WorldTransform& worldTransform_)
{
    // スケーリング行列を宣言
    Matrix4 matScale;
    // スケーリング倍率を行列に設定する
    matScale = { worldTransform_.scale_.x, 0.0f, 0.0f, 0.0f,//x
                 0.0f, worldTransform_.scale_.y, 0.0f, 0.0f,//y
                 0.0f, 0.0f, worldTransform_.scale_.z, 0.0f,//z
                 0.0f, 0.0f, 0.0f, 1.0f };

    // 合成用回転行列を宣言
    Matrix4 matRot;
    // 各軸用回転行列を宣言
    Matrix4 matRotX, matRotY, matRotZ;

    // Z軸回転行列の各要素を設定する
    matRotZ = { cos(worldTransform_.rotation_.z), sin(worldTransform_.rotation_.z), 0.0f, 0.0f,	//x
                -sin(worldTransform_.rotation_.z), cos(worldTransform_.rotation_.z), 0.0f, 0.0f,//y
                0.0f,0.0f, 1.0, 0.0f,																//z
                0.0f, 0.0f, 0.0f, 1.0f };
    // X軸回転行列の各要素を設定する
    matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,																//x
                0.0f, cos(worldTransform_.rotation_.x), sin(worldTransform_.rotation_.x), 0.0f,	//y
                0.0f, -sin(worldTransform_.rotation_.x), cos(worldTransform_.rotation_.x), 0.0f,//z
                0.0f, 0.0f, 0.0f, 1.0f };
    // Y軸回転行列の各要素を設定する
    matRotY = { cos(worldTransform_.rotation_.y), 0.0f, -sin(worldTransform_.rotation_.y), 0.0f,//x
                0.0f, 1.0f, 0.0f, 0.0f,	//y
                sin(worldTransform_.rotation_.y),0.0f, cos(worldTransform_.rotation_.y), 0.0f,	//z
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
                 worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z,1 };

    // 行列の合成
    worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
    worldTransform_.matWorld_ *= matScale;
    worldTransform_.matWorld_ *= matRot;
    worldTransform_.matWorld_ *= matTrans;
    if (worldTransform_.parent_) {
        worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
    }

    // 行列の転送
    worldTransform_.TransferMatrix();
}