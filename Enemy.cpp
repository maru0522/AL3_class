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

// staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::* Enemy::spFuncTable[])() = {
    &Enemy::Approach,       // 要素番号1
    &Enemy::Leave           // 要素番号2
};

void Enemy::Update()
{
    // デバッグテキスト
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);


    // 規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }

    (this->*spFuncTable[static_cast<int>(phase_)])();

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


void Enemy::Approach()
{
    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    move = { 0,0,-kApproachSpeed };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;
    // 規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }
}

void Enemy::Leave()
{
    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    move = { -kApproachSpeed,kApproachSpeed,0 };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;
}