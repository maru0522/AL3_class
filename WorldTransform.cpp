#include "WorldTransform.h"
#include "MathUtility.h"
#include <cmath>

void WorldTransform::Scale()
{
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };

	// 行列の転送
	TransferMatrix();
}

void WorldTransform::Rotation()
{
	// 合成用回転行列を宣言
	Matrix4 matRot;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ = { cos(rotation_.z), sin(rotation_.z), 0.0f, 0.0f,	//x
				-sin(rotation_.z), cos(rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,							//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列の各要素を設定する
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,							//x
				0.0f, cos(rotation_.x), sin(rotation_.x), 0.0f,	//y
				0.0f, -sin(rotation_.x), cos(rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y軸回転行列の各要素を設定する
	matRotY = { cos(rotation_.y), 0.0f, -sin(rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,							//y
				sin(rotation_.y),0.0f, cos(rotation_.y), 0.0f,	//z
				0.0f, 0.0f, 0.0f, 1.0f };

	// 各軸の回転行列を合成

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// 行列の転送
	TransferMatrix();
}

void WorldTransform::Translation()
{
	// 合成用回転行列を宣言
	Matrix4 matTrans;

	// 移動量を行列に設定する。
	matTrans = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 translation_.x,translation_.y,translation_.z,1 };

	// 行列の転送
	TransferMatrix();
}

void WorldTransform::Affin()
{
	// スケーリング行列を宣言
	Matrix4 matScale;
	// スケーリング倍率を行列に設定する
	matScale = { scale_.x, 0.0f, 0.0f, 0.0f,//x
				 0.0f, scale_.y, 0.0f, 0.0f,//y
				 0.0f, 0.0f, scale_.z, 0.0f,//z
				 0.0f, 0.0f, 0.0f, 1.0f };
	// 合成用回転行列を宣言
	Matrix4 matRot;
	// 各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	// Z軸回転行列の各要素を設定する
	matRotZ = { cos(rotation_.z), sin(rotation_.z), 0.0f, 0.0f,	//x
				-sin(rotation_.z), cos(rotation_.z), 0.0f, 0.0f,//y
				0.0f,0.0f, 1.0, 0.0f,															//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列の各要素を設定する
	matRotX = { 1.0f, 0.0f, 0.0f, 0.0f,															//x
				0.0f, cos(rotation_.x), sin(rotation_.x), 0.0f,	//y
				0.0f, -sin(rotation_.x), cos(rotation_.x), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };
	// Y軸回転行列の各要素を設定する
	matRotY = { cos(rotation_.y), 0.0f, -sin(rotation_.y), 0.0f,//x
				0.0f, 1.0f, 0.0f, 0.0f,	//y
				sin(rotation_.y),0.0f, cos(rotation_.y), 0.0f,//z
				0.0f, 0.0f, 0.0f, 1.0f };

	// 各軸の回転行列を合成

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	// 合成用回転行列を宣言
	Matrix4 matTrans;
	//matTrans = MathUtility::Matrix4Identity();

	// 移動量を行列に設定する。
	matTrans = { 1,0,0,0,
				 0,1,0,0,
				 0,0,1,0,
				 translation_.x,translation_.y,translation_.z,1 };

	// 行列の合成
	matWorld_ = MathUtility::Matrix4Identity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	// 行列の転送
	TransferMatrix();
}