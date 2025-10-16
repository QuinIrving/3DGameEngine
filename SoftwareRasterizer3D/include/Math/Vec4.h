#pragma once
#include <cmath>
#include "Vec3.h"
#include "Mat4.h"

class Quaternion;

template <typename T>
class Vec4 {
public:
	constexpr Vec4() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0)) {};
	constexpr Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};
	constexpr Vec4(Vec3<T> v, T w = T(1)) : x(v.x), y(v.y), z(v.z), w(w) {};

	// Vector addition & subtraction
	Vec4<T> operator+(const Vec4<T>& rhs) const;
	Vec4<T> operator-(const Vec4<T>& rhs) const;
	Vec4<T> operator-() const;
	Vec4<T>& operator+=(const Vec4<T>& rhs);
	Vec4<T>& operator-=(const Vec4<T>& rhs);

	// Scalar vector multiplication (vector scaling)
	Vec4<T> operator*(const T scalar) const;
	Vec4<T> operator/(const T scalar) const;
	Vec4<T>& operator*=(const T scalar);
	Vec4<T>& operator/=(const T scalar);

	Vec4<T> operator*(const Mat4<T>& rhs) const;
	Vec4<T>& operator*=(const Mat4<T>& rhs);

	Vec4<T> operator*(const Quaternion& q) const;
	Vec4<T>& operator*=(const Quaternion& q);

	// Equality, check if all element's are equal
	bool operator==(const Vec4<T>& rhs) const;
	bool operator!=(const Vec4<T>& rhs) const;

	constexpr T DotProduct(const Vec4<T>& rhs) const;
	constexpr static T DotProduct(const Vec4<T>& lhs, const Vec4<T>& rhs);

	Vec4<T> GetNormalized() const;
	T GetMagnitude() const;
	T GetMagnitudeSquared() const;

	void ScaleX(const T scaleFactor);
	void ScaleY(const T scaleFactor);
	void ScaleZ(const T scaleFactor);
	void ScaleW(const T scaleFactor);
	// T FastGetMagnitude() const; // probably something with the quake idea. Inverse square root approximation.

public:
	T x;
	T y;
	T z;
	T w;
};

template <typename T>
Vec4<T> Vec4<T>::operator+(const Vec4<T>& rhs) const {
	T newX = x + rhs.x;
	T newY = y + rhs.y;
	T newZ = z + rhs.z;
	T newW = w + rhs.w;

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
Vec4<T> Vec4<T>::operator-(const Vec4<T>& rhs) const {
	T newX = x - rhs.x;
	T newY = y - rhs.y;
	T newZ = z - rhs.z;
	T newW = w - rhs.w;

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
Vec4<T> Vec4<T>::operator-() const {
	return Vec4<T>(-x, -y, -z, -w);
}

template <typename T>
Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

template <typename T>
Vec4<T> Vec4<T>::operator*(const T scalar) const {
	T newX = x * scalar;
	T newY = y * scalar;
	T newZ = z * scalar;
	T newW = w * scalar;

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
Vec4<T> Vec4<T>::operator/(const T scalar) const {
	if (scalar == static_cast<T>(0)) {
		// Output some warning
		return Vec4<T>();
	}

	T newX = x / scalar;
	T newY = y / scalar;
	T newZ = z / scalar;
	T newW = w / scalar;

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
Vec4<T>& Vec4<T>::operator*=(const T scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}

template <typename T>
Vec4<T>& Vec4<T>::operator/=(const T scalar) {
	if (scalar == static_cast<T>(0)) {
		// Output some warning or probably throw an exception.
		return *this;
	}

	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}

template <typename T>
bool Vec4<T>::operator==(const Vec4<T>& rhs) const {
	return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

template <typename T>
bool Vec4<T>::operator!=(const Vec4<T>& rhs) const {
	return !(*this == rhs);
}

template<typename T>
inline constexpr T Vec4<T>::DotProduct(const Vec4<T>& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

template<typename T>
inline constexpr T Vec4<T>::DotProduct(const Vec4<T>& lhs, const Vec4<T>& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

template <typename T>
Vec4<T> Vec4<T>::GetNormalized() const {
	T mag = this->GetMagnitude();

	if (mag == 0) {
		// probably should do some exception here.
		return Vec4<T>();
	}

	T newX = x / mag;
	T newY = y / mag;
	T newZ = z / mag;
	T newW = w / mag;

	return Vec4<T>(newX, newY, newZ, newW);
}

template <typename T>
T Vec4<T>::GetMagnitude() const {
	return (std::sqrt(x * x + y * y + z * z + w * w));
}

template <typename T>
T Vec4<T>::GetMagnitudeSquared() const {
	return (x * x + y * y + z * z + w * w);
}

/*
template <typename T>
T Vec4<T>::FastGetMagnitude() const {
	// look up quake stuff
	return (x * x + y * y);
}*/

template <typename T>
void Vec4<T>::ScaleX(const T scaleFactor) {
	x *= scaleFactor;
}

template <typename T>
void Vec4<T>::ScaleY(const T scaleFactor) {
	y *= scaleFactor;
}

template <typename T>
void Vec4<T>::ScaleZ(const T scaleFactor) {
	z *= scaleFactor;
}

template <typename T>
void Vec4<T>::ScaleW(const T scaleFactor) {
	w *= scaleFactor;
}