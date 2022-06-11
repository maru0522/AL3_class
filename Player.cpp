#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Move()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	// �L�����N�^�[�̈ړ�����
	const float kCharacterSpeed = 0.2f;

	// �����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_W)) {
		move = { 0,kCharacterSpeed,0 };
	}
	if (input_->PushKey(DIK_S)) {
		move = { 0,-kCharacterSpeed,0 };
	}
	if (input_->PushKey(DIK_A)) {
		move = {-kCharacterSpeed,0,0};
	}
	if (input_->PushKey(DIK_D)) {
		move = { kCharacterSpeed,0,0 };
	}
	
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	// �ړ����E���W
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= 0.05f;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += 0.05f;
	}
	
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE)) {
		// �e�𔭎˂��A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		bullet_ = newBullet;
	}
}

void Player::Update()
{
	debugText_->SetPos(50,50);
	debugText_->Printf("player:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	Move();
	Rotate();
	Attack();
	if (bullet_) {
		bullet_->Update();
	}
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}