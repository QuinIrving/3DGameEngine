#pragma once
#include <array>
#include <stdexcept>
#include <algorithm>
#include "Vec3.h"

template <typename T>
class Vec4;

template <typename T>
class Mat4 {
public:
	class RowProxy { // MIGHT WANT TO CHANGE THIS TO STORE A VEC4 of the row, will see.
	public:
		RowProxy(T* rowData) : row(rowData) {}
		RowProxy(const T* rowData) : row(const_cast<T*>(rowData)) {}

		// Grab the column of a row
		T& operator[](int col); // read & write
		const T& operator[](int col) const; // only read

	private:
		T* row;
	};
public:
	Mat4();
	Mat4(T matrix[16]);
	Mat4(std::array<T, 16> matrix);

	T& operator()(int row, int col) { return values[row * 4 + col]; }
	Mat4<T> operator+(const Mat4<T>& rhs) const;
	Mat4<T>& operator+=(const Mat4<T>& rhs);
	Mat4<T> operator*(T factor) const; // scalar multiply;
	Mat4<T> operator*(const Mat4<T>& rhs) const; // matrix-matrix multiply;
	Mat4<T>& operator*=(T factor);
	Mat4<T>& operator*=(const Mat4<T>& rhs);
	RowProxy operator[](int row);
	const RowProxy operator[](int row) const;

	Mat4<T> GetTranspose() const;

	// only requires getting the Mat3 top left corner inverse, as the translations don't affect normals.
	Mat4<T> GetNormalMatrix() const;

	std::array<T, 4> GetValues();

	/*Mat4<T>& Translate(const Vec3<T>& v, T w = T(1));
	Mat4<T>& Rotate(const Vec3<T>& v, T w = T(1));
	Mat4<T>& Scale(const Vec3<T>& v, T w = T(1));

	static Mat4<T> Translate(Vec3<T> v, T w = T(1));
	static Mat4<T> Rotate(Vec3<T> v, T w = T(1));
	static Mat4<T> Scale(Vec3<T> v, T w = T(1));*/

	//Mat4<T>& Translate(const Vec4<T>& v);
	//Mat4<T>& Rotate(const Vec4<T>& v);
	//Mat4<T>& Scale(const Vec4<T>& v);

	static Mat4<T> Translate(Vec4<T> v);
	static Mat4<T> Rotate(Vec4<T> v);
	static Mat4<T> Scale(Vec4<T> v);

	static constexpr Mat4<T> GetIdentity();
	static constexpr Mat4<T> GetZero();



private:
	std::array<T, 16> values; // Row-major format m00, m01, m02, m03, m10, .. etc
};

template <typename T>
Mat4<T>::Mat4()
	: values{ {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
} } {}

template <typename T>
inline Mat4<T>::Mat4(T matrix[16])
{
	std::copy(matrix, matrix + 16, values.begin());
}

template <typename T>
inline Mat4<T>::Mat4(std::array<T, 16> matrix) : values (matrix) {}

template <typename T>
Mat4<T> Mat4<T>::operator+(const Mat4<T>& rhs) const {
	T addedData[16];
	for (int i = 0; i < 16; ++i) {
		addedData[i] = values[i] + rhs.values[i];
	}

	return Mat4(addedData);
}

template <typename T>
Mat4<T>& Mat4<T>::operator+=(const Mat4<T>& rhs) {
	for (int i = 0; i < 16; ++i) {
		values[i] += rhs.values[i];
	}

	return *this;
}

template <typename T>
Mat4<T> Mat4<T>::operator*(T factor) const {
	std::array<T, 16> multipliedData;
	for (int i = 0; i < 16; ++i) {
		multipliedData[i] = values[i] * factor;
	}

	return Mat4(multipliedData);
}


template <typename T>
Mat4<T>& Mat4<T>::operator*=(T factor) {
	for (int i = 0; i < 16; ++i) {
		values[i] *= factor;
	}

	return *this;
}

template <typename T>
Mat4<T>::RowProxy Mat4<T>::operator[](int row) {
	if (row < 0 || row >= 4) {
		throw std::out_of_range("row");
	}

	return RowProxy(&values[row * 4]);
}

template <typename T>
const Mat4<T>::RowProxy Mat4<T>::operator[](int row) const {
	if (row < 0 || row >= 4) {
		throw std::out_of_range("row");
	}

	return RowProxy(&values[row * 4]);
}

template <typename T>
std::array<T, 4> Mat4<T>::GetValues() {
	return values.data();
}

// Identity matrix I presume
template <typename T>
inline constexpr Mat4<T> Mat4<T>::GetIdentity() {
	return Mat4();
}

template <typename T>
inline constexpr Mat4<T> Mat4<T>::GetZero() {
	return Mat4<T>({ {0, 0, 0, 0,
				 0, 0, 0, 0,
				 0, 0, 0, 0,
				 0, 0, 0, 0} });
}

template <typename T>
inline T& Mat4<T>::RowProxy::operator[](int col) {
	if (col < 0 || col >= 4) {
		throw std::out_of_range("col");
	}

	return row[col];
}

template <typename T>
const T& Mat4<T>::RowProxy::operator[](int col) const {
	if (col < 0 || col >= 4) {
		throw std::out_of_range("col");
	}

	return row[col];
}

template <typename T>
Mat4<T> Mat4<T>::GetTranspose() const {
	Mat4<T> m;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] = (*this)[j][i];
		}
	}

	return m;
}

template <typename T>
Mat4<T> Mat4<T>::GetNormalMatrix() const {
	// only the upper 3x3 matters as for a normal translation doesn't come into play, only the rotation and scaling.

	T a = (*this)[0][0], b = (*this)[0][1], c = (*this)[0][2];
	T d = (*this)[1][0], e = (*this)[1][1], f = (*this)[1][2];
	T g = (*this)[2][0], h = (*this)[2][1], i = (*this)[2][2];

	// Determinant:
	T det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
	T invDet = 1.f / det;

	// Cofactor matrix (inverse)
	Mat4<T> inv;
	inv[0][0] = (e * i - f * h) * invDet;
	inv[0][1] = -(b * i - c * h) * invDet;
	inv[0][2] = (b * f - c * e) * invDet;

	inv[1][0] = -(d * i - f * g) * invDet;
	inv[1][1] = (a * i - c * g) * invDet;
	inv[1][2] = -(a * f - c * d) * invDet;

	inv[2][0] = (d * h - e * g) * invDet;
	inv[2][1] = -(a * h - b * g) * invDet;
	inv[2][2] = (a * e - b * d) * invDet;

	// Transpose, only the 3x3 not the translation parts.
	Mat4<T> normal;
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			normal[r][c] = inv[c][r];
		}
	}

	// not sure if we even need this but will keep the factor there for consistency.
	normal[3][3] = 1;

	return normal;
}

// want some way to change the idea of getting a direction vs a position vector (0, vs 1 on the vec4 version when multiplying)

/*
Transformation functions for rotation, scaling, translation etc. Keep them static.
perhaps also the member functions to apply the transformation on the current matrix.
- Rotation x, rotation y, rotation z, rotation axis (with a vec3 axis and the angle)
- Scale via Vec3 or 3 defined points of type T
- Translation similarly Vec3 or 3 defined values

- matrix-vector multiply.

- Mat4 could contain our transformation matrix that we are applying our changes to
and then transform the point/direction via that final transformation
- ^ this would mean we could then call TransformPoint or TransformDirection for it.

- Model view projection. projection * view * model should be in Camera class, and be 
pre-computed once a frame to be used on all vertices in my pipeline.


A Mat4 transformation matrix should be one in each object
so that we can then apply to the vertices each of them to properly move them around the scene.

- Camera should have the view and projection matrix.
*/
