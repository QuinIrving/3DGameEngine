#pragma once
#include "Vec4.h"
#include "Mat4.h"

template <typename T>
Vec4<T> Vec4<T>::operator*(const Mat4<T>& rhs) const {
	T newX = DotProduct(Vec4<T>(rhs[0][0], rhs[1][0], rhs[2][0], rhs[3][0]));
	T newY = DotProduct(Vec4<T>(rhs[0][1], rhs[1][1], rhs[2][1], rhs[3][1]));
	T newZ = DotProduct(Vec4<T>(rhs[0][2], rhs[1][2], rhs[2][2], rhs[3][2]));
	T newW = DotProduct(Vec4<T>(rhs[0][3], rhs[1][3], rhs[2][3], rhs[3][3]));

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
Vec4<T>& Vec4<T>::operator*=(const Mat4<T>& rhs) {
	T newX = DotProduct(Vec4<T>(rhs[0][0], rhs[1][0], rhs[2][0], rhs[3][0]));
	T newY = DotProduct(Vec4<T>(rhs[0][1], rhs[1][1], rhs[2][1], rhs[3][1]));
	T newZ = DotProduct(Vec4<T>(rhs[0][2], rhs[1][2], rhs[2][2], rhs[3][2]));
	T newW = DotProduct(Vec4<T>(rhs[0][3], rhs[1][3], rhs[2][3], rhs[3][3]));

	x = newX;
	y = newY;
	z = newZ;
	w = newW;

	return *this;
}

template <typename T>
Mat4<T> Mat4<T>::operator*(const Mat4<T>& rhs) const {
	std::array<T, 16> multipliedData;

	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			multipliedData[r * 4 + c] = Vec4<T>::DotProduct(Vec4<T>((*this)[r][0], (*this)[r][1], (*this)[r][2], (*this)[r][3]),
				Vec4<T>(rhs[0][c], rhs[1][c], rhs[2][c], rhs[3][c]));
		}
	}

	return Mat4(multipliedData);
}

template <typename T>
Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& rhs) {
	std::array<T, 16> multipliedData;

	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			multipliedData[r * 4 + c] = Vec4<T>::DotProduct(Vec4<T>((*this)[r][0], (*this)[r][1], (*this)[r][2], (*this)[r][3]),
				Vec4<T>(rhs[0][c], rhs[1][c], rhs[2][c], rhs[3][c]));
		}
	}

	values = multipliedData;

	return *this;
}
/*
template<typename T>
inline Mat4<T>& Translate(const Vec3<T>& v, T w = T(1)) {
	return *this;
}

template<typename T>
inline Mat4<T>& Rotate(const Vec3<T>& v, T w = T(1)) {
	return *this;
}

template<typename T>
inline Mat4<T>& Scale(const Vec3<T>& v, T w = T(1)) {
	return *this;
}

template<typename T>
inline static Mat4<T> Translate(Vec3<T> v, T w = T(1)) {
	return Mat4<T>();
}

template<typename T>
inline static Mat4<T> Rotate(Vec3<T> v, T w = T(1)) {
	return Mat4<T>();
}

template<typename T>
inline static Mat4<T> Scale(Vec3<T> v, T w = T(1)) {
	return Mat4<T>();
}*/
/*
template<typename T>
inline Mat4<T>& Mat4<T>::Translate(const Vec4<T>& v)
{
	return *this;
}


template <typename T>
inline Mat4<T>& Mat4<T>::Rotate(const Vec4<T>& v) {
	return *this;
}

template <typename T>
inline Mat4<T>& Mat4<T>::Scale(const Vec4<T>& v) {
	return *this;
}
*/

template <typename T>
inline Mat4<T> Mat4<T>::Translate(Vec4<T> v) {
	Mat4<T> m = Mat4<T>();
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
	m[3][3] = v.w;
	return m;
}

template <typename T>
inline Mat4<T> Mat4<T>::Rotate(Vec4<T> v) {
	return Mat4<T>();
}

template <typename T>
inline Mat4<T> Mat4<T>::Scale(Vec4<T> v) {
	Mat4<T> m = Mat4<T>();
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	return m;
}