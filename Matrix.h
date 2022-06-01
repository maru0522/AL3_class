#pragma once

#define _COL 4		// 列数 x
#define _ROW 4		// 行数 y

class Matrix
{
private:
	// 行列
	float matrix[_ROW][_COL];

	void mul(Matrix* A, Matrix* B, Matrix* C);

public:
	// コンストラクタ
	Matrix();
	// オーバーロード 成分指定
	Matrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	
	// 代入演算子オーバーロード
	Matrix& operator*(Matrix& matrix2);
	Matrix& operator*=(Matrix& matrix2);

	// 静的配列での運用を想定しているためデストラクタ無し。
};