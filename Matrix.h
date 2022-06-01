#pragma once

#define _COL 4		// �� x
#define _ROW 4		// �s�� y

class Matrix
{
private:
	// �s��
	float matrix[_ROW][_COL];

	void mul(Matrix* A, Matrix* B, Matrix* C);

public:
	// �R���X�g���N�^
	Matrix();
	// �I�[�o�[���[�h �����w��
	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	
	// ������Z�q�I�[�o�[���[�h
	Matrix& operator*(Matrix& matrix2);
	Matrix& operator*=(Matrix& matrix2);

	// �ÓI�z��ł̉^�p��z�肵�Ă��邽�߃f�X�g���N�^�����B
};