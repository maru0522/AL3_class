#include "Matrix.h"


// コンストラクタ
Matrix::Matrix()
{
}

// オーバーロード 成分指定
Matrix::Matrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	matrix[0][0] = m00;
	matrix[0][1] = m01;
	matrix[0][2] = m02;
	matrix[0][3] = m03;
	matrix[1][0] = m10;
	matrix[1][1] = m11;
	matrix[1][2] = m12;
	matrix[1][3] = m13;
	matrix[2][0] = m20;
	matrix[2][1] = m21;
	matrix[2][2] = m22;
	matrix[2][3] = m23;
	matrix[3][0] = m30;
	matrix[3][1] = m31;
	matrix[3][2] = m32;
	matrix[3][3] = m33;
}


void Matrix::mul(Matrix* A, Matrix* B, Matrix* C)
{
	for (int y = 0; y < _ROW; y++) {
		for (int x = 0; x < _COL; x++) {
			float sum = 0;
			for (int i = 0; i < _ROW; i++) {
				sum += A->matrix[y][i] * B->matrix[i][x];
			}
			C->matrix[y][x] = sum;
		}
	}
}

Matrix& Matrix::operator*(Matrix& matrix2)
{
	Matrix result;
	mul(this, &matrix2, &result);
	return result;
}

Matrix& Matrix::operator*=(Matrix& matrix2)
{
	Matrix result;
	mul(this, &matrix2, &result);
	for (int y = 0; y < _ROW; y++) {
		for (int x = 0; x < _COL; x++) {
			this->matrix[y][x] = result.matrix[y][x];
		}
	}
	return *this;
}