#include "WorldTransform.h"
#include "MathUtility.h"
#include <cmath>

void WorldTransform::Scale()
{
	// �X�P�[�����O�s���錾
	Matrix4 matScale;
	// �X�P�[�����O�{�����s��ɐݒ肷��
	matScale = { scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	// �s��̓]��
	TransferMatrix();
}

void WorldTransform::Rotation()
{
	// �����p��]�s���錾
	Matrix4 matRot;
	// �e���p��]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	// Z����]�s��̊e�v�f��ݒ肷��
	matRotZ = { cos(rotation_.z), sin(rotation_.z), 0.0f, 0.0f,	//x
				-sin(rotation_.z), cos(rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,							//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X����]�s��̊e�v�f��ݒ肷��
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,							//x
				0.0f, cos(rotation_.x), sin(rotation_.x), 0.0f,	//y
				0.0f, -sin(rotation_.x), cos(rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y����]�s��̊e�v�f��ݒ肷��
	matRotY = { cos(rotation_.y), 0.0f, -sin(rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,							//y
				sin(rotation_.y),0.0f, cos(rotation_.y), 0.0f,	//z
				0.0f, 0.0f, 0.0f, 1.0f };

	// �e���̉�]�s�������

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// �s��̓]��
	TransferMatrix();
}

void WorldTransform::Translation()
{
	// �����p��]�s���錾
	Matrix4 matTrans;

	// �ړ��ʂ��s��ɐݒ肷��B
	matTrans = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 translation_.x,translation_.y,translation_.z,1 };

	// �s��̓]��
	TransferMatrix();
}

void WorldTransform::Affin()
{
	// �X�P�[�����O�s���錾
	Matrix4 matScale;
	// �X�P�[�����O�{�����s��ɐݒ肷��
	matScale = { scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };
	// �����p��]�s���錾
	Matrix4 matRot;
	// �e���p��]�s���錾
	Matrix4 matRotX, matRotY, matRotZ;

	// Z����]�s��̊e�v�f��ݒ肷��
	matRotZ = { cos(rotation_.z), sin(rotation_.z), 0.0f, 0.0f,	//x
				-sin(rotation_.z), cos(rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,															//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X����]�s��̊e�v�f��ݒ肷��
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,															//x
				0.0f, cos(rotation_.x), sin(rotation_.x), 0.0f,	//y
				0.0f, -sin(rotation_.x), cos(rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y����]�s��̊e�v�f��ݒ肷��
	matRotY = { cos(rotation_.y), 0.0f, -sin(rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,	//y
				sin(rotation_.y),0.0f, cos(rotation_.y), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };

	// �e���̉�]�s�������

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// �����p��]�s���錾
	Matrix4 matTrans;
	//matTrans = MathUtility::Matrix4Identity();

	// �ړ��ʂ��s��ɐݒ肷��B
	matTrans = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 translation_.x,translation_.y,translation_.z,1 };

	// �s��̍���
	matWorld_ = MathUtility::Matrix4Identity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	// �s��̓]��
	TransferMatrix();
}