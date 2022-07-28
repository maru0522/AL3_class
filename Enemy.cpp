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


void Enemy::Update()
{
    // �f�o�b�O�e�L�X�g
    debugText_->SetPos(50, 70);
    debugText_->Printf("Enemy:(%f,%f,%f)",
                       worldTransform_.translation_.x,
                       worldTransform_.translation_.y,
                       worldTransform_.translation_.z);

    // �L�����N�^�[�̈ړ��x�N�g��
    Vector3 move = { 0,0,0 };

    // �L�����N�^�[�̈ړ�����
    const float kCharacterSpeed = 0.2f;

    move = { 0,0,-kCharacterSpeed };

    worldTransform_.translation_.x += move.x;
    worldTransform_.translation_.y += move.y;
    worldTransform_.translation_.z += move.z;

    //���[���h�s��X�V
    worldTransform_.UpdateMatrix();
}


void Enemy::Draw(ViewProjection viewProjection)
{
    model_->Draw(worldTransform_, viewProjection, textureHandle_);
}