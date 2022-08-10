#include "Enemy.h"
#include "Calc.h"

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

    PhaseInitApproach();

    worldTransform_.Initialize();
}

void Enemy::Fire()
{
    // �e�𔭎˂��A������
    std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
    newBullet->Initialize(model_, worldTransform_.translation_, { 0.0f,0.0f,-1.0f });

    // �e��o�^����
    bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseInitApproach()
{
    // ���˃^�C�}��������
    fireTimer = kFireInterval;
}

void Enemy::PhaseApproach()
{
    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    move = { 0,0,-kApproachSpeed };
    // �ړ��i�x�N�g�������Z�j
    worldTransform_.translation_ += move;
    
    // ���˃^�C�}�J�E���g�_�E��
    fireTimer--;

    // �w�莞�ԂɒB����
    if (fireTimer <= 0) {
        Fire();
        // ���˃^�C�}�̏�����
        PhaseInitApproach();
    }

    // �K��̈ʒu�ɓ��B�����痣�E
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }
}

void Enemy::PhaseLeave()
{
    const float kApproachSpeed = 0.2f;

    Vector3 move = { -kApproachSpeed,kApproachSpeed,0 };
    // �ړ��i�x�N�g�������Z�j
    worldTransform_.translation_ += move;
}


void Enemy::Update()
{
    // �f�o�b�O�e�L�X�g
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

    // �e�X�V
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Update();
    }

    //���[���h�s��X�V
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
    for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
        bullet->Draw(viewProjection);
    }
}