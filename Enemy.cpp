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
    // NULL�|�C���^�`�F�b�N
    assert(model);

    // �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    model_ = model;
    textureHandle_ = textureHandle;

    // �V���O���g���C���X�^���X�̎擾
    debugText_ = DebugText::GetInstance();

    worldTransform_.translation_.z = 30;

    worldTransform_.Initialize();
}

void Enemy::Update()
{
    // �f�o�b�O�e�L�X�g
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);


    // �K��̈ʒu�ɓ��B�����痣�E
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }

    //���[���h�s��X�V
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

// EnginApproach�N���X�̎���
void EnemyApproach::Up(Enemy* pEnemy)
{
    std::cout << "Approach -> Leave" << std::endl;
    pEnemy->ChangeState(new EnemyLeave());
}

void EnemyApproach::Down(Enemy* pEnemy)
{
    std::cout << "(Approach) �ύX�Ȃ�" << std::endl;
    pEnemy->ChangeState(new EnemyApproach());
}

void EnemyApproach::ShowState()
{
    std::cout << "state : Approach" << std::endl;
}

void EnemyLeave::Up(Enemy* pEnemy)
{
    std::cout << "(Leave) �ύX�Ȃ�" << std::endl;
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
