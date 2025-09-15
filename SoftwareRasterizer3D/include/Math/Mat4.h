#pragma once
#include <array>

template <typename T>
class Mat4 {
public:
	class RowProxy {
	public:
		RowProxy(T* rowData) : row(rowData) {}

		// Grab the column of a row
		T& operator[](int col); // read & write
		const T& operator[](int col) const; // only read

	private:
		T* row;
	};
public:
	Mat4();
	Mat4(T matrix[16]);

	T& operator()(int row, int col) { return values[row * 4 + col]; }
	Mat4<T> operator+(const Mat4<T>& rhs);
	Mat4<T>& operator+=(const Mat4<T>& rhs);
	Mat4<T> operator*(T factor); // scalar multiply;
	Mat4<T> operator*(const Mat4<T>& rhs); // matrix-matrix multiply;
	Mat4<T>& operator*=(T factor);
	Mat4<T>& operator*=(const Mat4<T>& rhs);
	RowProxy operator[](int row);
	const RowProxy operator[](int row) const;

	T* GetValues();

	static constexpr Mat4<T> GetIdentity();
	static constexpr Mat4<T> GetZero();



private:
	std::array<T, 16> values; // Row-major format m00, m01, m02, m03, m10, .. etc
};

template<typename T>
Mat4<T>::Mat4()
	: values{ {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
} } {}

template<typename T>
inline Mat4<T>::Mat4(T matrix[16])
{
	values = matrix;
}

template<typename T>
Mat4<T> Mat4<T>::operator+(const Mat4<T>& rhs) {
	T addedData[16];
	for (int i = 0; i < 16; ++i) {
		addedData[i] = values[i] + rhs.values[i];
	}

	return Mat4(addedData);
}

template<typename T>
Mat4<T>& Mat4<T>::operator+=(const Mat4<T>& rhs) {
	for (int i = 0; i < 16; ++i) {
		values[i] += rhs.values[i];
	}

	return *this;
}

template<typename T>
Mat4<T> Mat4<T>::operator*(T factor) {
	T multipliedData[16];
	for (int i = 0; i < 16; ++i) {
		multipliedData[i] = values[i] * factor;
	}

	return Mat4(multipliedData);
}

template<typename T>
Mat4<T> Mat4<T>::operator*(const Mat4<T>& rhs) {
	T multipliedData[16];
	for (int i = 0; i < 16; ++i) {
		multipliedData[i] = values[i] * rhs.values[i];
	}

	return Mat4(multipliedData);
}

template<typename T>
Mat4<T>& Mat4<T>::operator*=(T factor) {
	for (int i = 0; i < 16; ++i) {
		values[i] *= factor;
	}

	return *this;
}

template<typename T>
Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& rhs) {
	for (int i = 0; i < 16; ++i) {
		values[i] *= rhs.values[i];
	}

	return *this;
}

template<typename T>
Mat4<T>::RowProxy Mat4<T>::operator[](int row) {
	if (row < 0 || row >= 4) {
		// throw an error
	}

	return RowProxy(&values[row * 4]);
}

template<typename T>
const Mat4<T>::RowProxy Mat4<T>::operator[](int row) const {
	return RowProxy(&values[row * 4]);
}

template<typename T>
T* Mat4<T>::GetValues() {
	return values.data();
}

// Identity matrix I presume
template<typename T>
inline constexpr Mat4<T> Mat4<T>::GetIdentity() {
	return Mat4();
}

template<typename T>
inline constexpr Mat4<T> Mat4<T>::GetZero() {
	return Mat4<T>({ {0, 0, 0, 0,
				 0, 0, 0, 0,
				 0, 0, 0, 0,
				 0, 0, 0, 0} });
}

template<typename T>
inline T& Mat4<T>::RowProxy::operator[](int col) {
	if (col < 0 || col >= 4) {
		// throw an error;
	}

	return row[col];
}

template<typename T>
const T& Mat4<T>::RowProxy::operator[](int col) const {
	if (col < 0 || col >= 4) {
		// throw an error;
	}

	return row[col];
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
