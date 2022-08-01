#include "Enemy.h"

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

// static�Ő錾���������o�֐��|�C���^�e�[�u���̎���
void (Enemy::* Enemy::spFuncTable[])() = {
    &Enemy::Approach,       // �v�f�ԍ�1
    &Enemy::Leave           // �v�f�ԍ�2
};

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

    (this->*spFuncTable[static_cast<int>(phase_)])();

    //���[���h�s��X�V
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
    // �ړ��i�x�N�g�������Z�j
    worldTransform_.translation_ += move;
    // �K��̈ʒu�ɓ��B�����痣�E
    if (worldTransform_.translation_.z < 0.0f) {
        phase_ = Phase::Leave;
    }
}

void Enemy::Leave()
{
    const float kApproachSpeed = 0.2f;
    Vector3 move = { 0,0,0 };

    move = { -kApproachSpeed,kApproachSpeed,0 };
    // �ړ��i�x�N�g�������Z�j
    worldTransform_.translation_ += move;
}