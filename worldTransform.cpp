#include "WorldTransform.h"
#include "MathUtility.h"
#include <cmath>

void WorldTransform::SetMatrixScale(Matrix4& matScale_)
{
	matScale_ = { scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };
}

void WorldTransform::SetMatrixRotateX(Matrix4& matRotX_)
{
	matRotX_ = { 1.0f, 0.0f, 0.0f, 0.0f,						//x
				0.0f, cos(rotation_.x), sin(rotation_.x), 0.0f,	//y
				0.0f, -sin(rotation_.x), cos(rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
}

void WorldTransform::SetMatrixRotateY(Matrix4& matRotY_)
{
	matRotY_ = { cos(rotation_.y), 0.0f, -sin(rotation_.y), 0.0f,	//x
				0.0f, 1.0f, 0.0f, 0.0f,								//y
				sin(rotation_.y),0.0f, cos(rotation_.y), 0.0f,		//z
				0.0f, 0.0f, 0.0f, 1.0f };
}

void WorldTransform::SetMatrixRotateZ(Matrix4& matRotZ_)
{
	matRotZ_ = { cos(rotation_.z), sin(rotation_.z), 0.0f, 0.0f,	//x
				-sin(rotation_.z), cos(rotation_.z), 0.0f, 0.0f,	//y
				0.0f,0.0f, 1.0, 0.0f,								//z
				0.0f, 0.0f, 0.0f, 1.0f };
}

void WorldTransform::SetMatrixRotate(Matrix4& matRot_, Matrix4 matRotX_, Matrix4 matRotY_, Matrix4 matRotZ_)
{
	matRot_ = matRotZ_;
	matRot_ *= matRotX_;
	matRot_ *= matRotY_;
}

void WorldTransform::SetMatrixTranslate(Matrix4& matTrans_)
{
	matTrans_ = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 translation_.x,translation_.y,translation_.z,1 };
}

void WorldTransform::SetMatrixWorldTransform(Matrix4& worldTransform_, Matrix4 matScale_, Matrix4 matRot_, Matrix4 matTrans_)
{
	matWorld_ *= matScale_;
	matWorld_ *= matRot_;
	matWorld_ *= matTrans_;
}


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

void WorldTransform::UpdateMatrix()
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