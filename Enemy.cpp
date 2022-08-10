#include "Enemy.h"
#include "Calc.h"

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

    PhaseInitApproach();

    worldTransform_.Initialize();
}

void Enemy::Fire()
{
    // 弾を発射し、初期化
    std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
    newBullet->Initialize(model_, worldTransform_.translation_, { 0.0f,0.0f,-1.0f });

    // 弾を登録する
    bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseInitApproach()
{
    // 発射タイマを初期化
    fireTimer = kFireInterval;
}

void Enemy::PhaseApproach()
{
    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    move = { 0,0,-kApproachSpeed };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;
    
    // 発射タイマカウントダウン
    fireTimer--;

    // 指定時間に達した
    if (fireTimer <= 0) {
        Fire();
        // 発射タイマの初期化
        PhaseInitApproach();
    }

    // 規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }
}

void Enemy::PhaseLeave()
{
    const float kApproachSpeed = 0.2f;

    Vector3 move = { -kApproachSpeed,kApproachSpeed,0 };
    // 移動（ベクトルを加算）
    worldTransform_.translation_ += move;
}


void Enemy::Update()
{
    // デバッグテキスト
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);

    switch (phase_) {
        case Phase::Approach:
        default:
            PhaseApproach();
            break;
        case Phase::Leave:
            PhaseLeave();
            break;
    }

    // 弾更新
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Update();
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection);
    }
}