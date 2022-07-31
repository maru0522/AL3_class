#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
    // NULLポインタチェック
    assert(model);

    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = model;
    textureHandle_ = textureHandle;

    // シングルトンインスタンスの取得
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_.z = 30;

    worldTransform_.Initialize();
}


void Enemy::Update()
{
    // デバッグテキスト
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);

    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    switch (phase_) {
        case Phase::Approach:
        default:
            move = { 0,0,-kApproachSpeed };
            // 移動（ベクトルを加算）
            worldTransform_.translation_ += move;
            // 規定の位置に到達したら離脱
            if (worldTransform_.translation_.z < 0.0f) {
                phase_ = Phase::Leave;
            }
            break;
        case Phase::Leave:
            Vector3 move = { -kApproachSpeed,kApproachSpeed,0 };
            // 移動（ベクトルを加算）
            worldTransform_.translation_ += move;
            break;
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
}