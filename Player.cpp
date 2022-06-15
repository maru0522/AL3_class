#include "Player.h"
#include "Calc.h"

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
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = Calc::DotVecMat(velocity, worldTransform_.matWorld_);

		// �e�𔭎˂��A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		// �e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bulet) {
		return bulet->isDead();
					   });

	// �f�o�b�O�e�L�X�g
	debugText_->SetPos(50,50);
	debugText_->Printf("player:(%f,%f,%f)",
					   worldTransform_.translation_.x,
					   worldTransform_.translation_.y,
					   worldTransform_.translation_.z);

	// �ړ�
	Move();
	// ��]
	Rotate();
	// �e����
	Attack();

	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//���[���h�s��X�V
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}