#include "Enemy.h"
#include <iostream>



Enemy::Enemy()
{
    state = new EnemyApproach();
}

Enemy::~Enemy()
{
    delete state;
}

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


    // 規定の位置に到達したら離脱
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }

    //ワールド行列更新
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Up()
{
    state->Up(this);
}

void Enemy::Down()
{
    state->Down(this);
}

void Enemy::ChangeState(EnemyState* newState)
{
    delete state;
    state = newState;
}

void Enemy::ShowNowState()
{
    state->ShowState();
}

// EnginApproachクラスの実装
void EnemyApproach::Up(Enemy* pEnemy)
{
    std::cout << "Approach -> Leave" << std::endl;
    pEnemy->ChangeState(new EnemyLeave());
}

void EnemyApproach::Down(Enemy* pEnemy)
{
    std::cout << "(Approach) 変更なし" << std::endl;
    pEnemy->ChangeState(new EnemyApproach());
}

void EnemyApproach::ShowState()
{
    std::cout << "state : Approach" << std::endl;
}

void EnemyLeave::Up(Enemy* pEnemy)
{
    std::cout << "(Leave) 変更なし" << std::endl;
    pEnemy->ChangeState(new EnemyApproach());
}

void EnemyLeave::Down(Enemy* pEnemy)
{
    std::cout << "Leave -> Approach" << std::endl;
    pEnemy->ChangeState(new EnemyLeave());
}

void EnemyLeave::ShowState()
{
    std::cout << "state : Leave" << std::endl;
}
