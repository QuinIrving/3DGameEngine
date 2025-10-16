#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Quaternion.h"
#include <cmath>
#include "GeneralMath.h"

template <typename T>
inline Vec4<T> Vec4<T>::operator*(const Mat4<T>& rhs) const {
	T newX = DotProduct(Vec4<T>(rhs[0][0], rhs[1][0], rhs[2][0], rhs[3][0]));
	T newY = DotProduct(Vec4<T>(rhs[0][1], rhs[1][1], rhs[2][1], rhs[3][1]));
	T newZ = DotProduct(Vec4<T>(rhs[0][2], rhs[1][2], rhs[2][2], rhs[3][2]));
	T newW = DotProduct(Vec4<T>(rhs[0][3], rhs[1][3], rhs[2][3], rhs[3][3]));

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
inline Vec4<T>& Vec4<T>::operator*=(const Mat4<T>& rhs) {
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
inline Mat4<T> Mat4<T>::operator*(const Mat4<T>& rhs) const {
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
inline Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& rhs) {
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
	Mat4<T> rotX, rotY, rotZ;
	
	// transform to radians from degrees.
	float rX = v.x * DEGREE_TO_RADIANS;
	float rY = v.y * DEGREE_TO_RADIANS;
	float rZ = v.z * DEGREE_TO_RADIANS;

	
	// rot X
	rotX[0][0] = 1;
	rotX[0][1] = 0;
	rotX[0][2] = 0;

	rotX[1][0] = 0;
	rotX[1][1] = cos(rX);
	rotX[1][2] = -sin(rX);

	rotX[2][0] = 0;
	rotX[2][1] = sin(rX);
	rotX[2][2] = cos(rX);
	

	//Rot Y
	rotY[0][0] = cos(rY);
	rotY[0][1] = 0;
	rotY[0][2] = -sin(rY);

	rotY[1][0] = 0;
	rotY[1][1] = 1;
	rotY[1][2] = 0;

	rotY[2][0] = sin(rY);
	rotY[2][1] = 0;
	rotY[2][2] = cos(rY);
	

	// Rot Z
	rotZ[0][0] = cos(rZ);
	rotZ[0][1] = sin(rZ);
	rotZ[0][2] = 0;

	rotZ[1][0] = -sin(rZ);
	rotZ[1][1] = cos(rZ);
	rotZ[1][2] = 0;

	rotZ[2][0] = 0;
	rotZ[2][1] = 0;
	rotZ[2][2] = 1;

	return rotX * rotY * rotZ;
}

template <typename T>
inline Mat4<T> Mat4<T>::Scale(Vec4<T> v) {
	Mat4<T> m = Mat4<T>();
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	return m;
}

template <typename T>
inline Vec3<T> Vec3<T>::operator*(const Quaternion& q) const {
	Vec4<float> v = *this;
	v.w = 0;
	Quaternion vQuat = Quaternion(v);
	Quaternion result = q * vQuat * ~q;
	const Vec4<float>& delta = result.GetDelta();
	return Vec3<float>(delta.x, delta.y, delta.z);
}

template <typename T>
inline Vec3<T>& Vec3<T>::operator*=(const Quaternion& q) {
	Vec4<float> v = *this;
	v.w = 0;
	Quaternion vQuat = Quaternion(v);
	*this = Vec3<float>((q * vQuat * ~q).GetDelta());
	return *this;
}

template <typename T>
inline Vec4<T> Vec4<T>::operator*(const Quaternion& q) const {

	Quaternion vQuat = Quaternion(Vec4<float>(this->x, this->y, this->z, 0.f));
	Quaternion result = q * vQuat * ~q;
	const Vec4<float>& delta = result.GetDelta();
	return Vec4<float>(delta.x, delta.y, delta.z, 1.f);
}

template <typename T>
inline Vec4<T>& Vec4<T>::operator*=(const Quaternion& q) {
	Quaternion vQuat = Quaternion(Vec4<float>(this->x, this->y, this->z, 0.f));
	this = (q * vQuat * ~q).GetDelta();
	return *this;
}
