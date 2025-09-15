#pragma once
#include <cmath>

template <typename T>
class Vec3 {
public:
	constexpr Vec3() : x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)) {};
	constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {};

	// Vector addition & subtraction
	Vec3<T> operator+(const Vec3<T>& rhs) const;
	Vec3<T> operator-(const Vec3<T>& rhs) const;
	Vec3<T>& operator+=(const Vec3<T>& rhs);
	Vec3<T>& operator-=(const Vec3<T>& rhs);

	// Scalar vector multiplication (vector scaling)
	Vec3<T> operator*(const T scalar) const;
	Vec3<T> operator/(const T scalar) const;
	Vec3<T>& operator*=(const T scalar);
	Vec3<T>& operator/=(const T scalar);

	// Equality, check if all element's are equal
	bool operator==(const Vec3<T>& rhs) const;
	bool operator!=(const Vec3<T>& rhs) const;

	constexpr T DotProduct(const Vec3<T>& rhs) const;
	constexpr static T DotProduct(const Vec3<T>& lhs, const Vec3<T>& rhs);

	constexpr Vec3<T> CrossProduct(const Vec3<T>& rhs) const;
	constexpr static Vec3<T> CrossProduct(const Vec3<T>& lhs, const Vec3<T>& rhs);

	Vec3<T> GetNormalized() const;
	T GetMagnitude() const;
	T GetMagnitudeSquared() const;

	void ScaleX(const T scaleFactor);
	void ScaleY(const T scaleFactor);
	void ScaleZ(const T scaleFactor);
	// T FastGetMagnitude() const; // probably something with the quake idea. Inverse square root approximation.

public:
	T x;
	T y;
	T z;
};

template <typename T>
Vec3<T> Vec3<T>::operator+(const Vec3<T>& rhs) const {
	T newX = x + rhs.x;
	T newY = y + rhs.y;
	T newZ = z + rhs.z;

	return Vec3<T>(newX, newY, newZ);
}

template <typename T>
Vec3<T> Vec3<T>::operator-(const Vec3<T>& rhs) const {
	T newX = x - rhs.x;
	T newY = y - rhs.y;
	T newZ = z - rhs.z;

	return Vec3<T>(newX, newY, newZ);
}

template <typename T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator*(const T scalar) const {
	T newX = x * scalar;
	T newY = y * scalar;
	T newZ = z * scalar;

	return Vec3<T>(newX, newY, newZ);
}

template <typename T>
Vec3<T> Vec3<T>::operator/(const T scalar) const {
	if (scalar == static_cast<T>(0)) {
		// Output some warning
		return Vec3<T>();
	}

	T newX = x / scalar;
	T newY = y / scalar;
	T newZ = z / scalar;

	return Vec3<T>(newX, newY, newZ);
}

template <typename T>
Vec3<T>& Vec3<T>::operator*=(const T scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator/=(const T scalar) {
	if (scalar == static_cast<T>(0)) {
		// Output some warning or probably throw an exception.
		return *this;
	}

	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

template <typename T>
bool Vec3<T>::operator==(const Vec3<T>& rhs) const {
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

template <typename T>
bool Vec3<T>::operator!=(const Vec3<T>& rhs) const {
	return !(*this == rhs);
}

template<typename T>
inline constexpr T Vec3<T>::DotProduct(const Vec3<T>& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

template<typename T>
inline constexpr T Vec3<T>::DotProduct(const Vec3<T>& lhs, const Vec3<T>& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename T>
inline constexpr Vec3<T> Vec3<T>::CrossProduct(const Vec3<T>& rhs) const {
	// positive orientation a.k.a right handed system
	T newX = (y * rhs.z) - (z * rhs.y);
	T newY = (z * rhs.x) - (x * rhs.z);
	T newZ = (x * rhs.y) - (y * rhs.x);

	return Vec3<T>(newX, newY, newZ);
}

template<typename T>
inline constexpr Vec3<T> Vec3<T>::CrossProduct(const Vec3<T>& lhs, const Vec3<T>& rhs) { 
	T newX = (lhs.y * rhs.z) - (lhs.z * rhs.y);
	T newY = (lhs.z * rhs.x) - (lhs.x * rhs.z);
	T newZ = (lhs.x * rhs.y) - (lhs.y * rhs.x);

	return Vec3<T>(newX, newY, newZ);
}



template <typename T>
Vec3<T> Vec3<T>::GetNormalized() const {
	T mag = this->GetMagnitude();

	if (mag == 0) {
		// probably should do some exception here.
		return Vec3<T>();
	}

	T newX = x / mag;
	T newY = y / mag;
	T newZ = z / mag;

	return Vec3<T>(newX, newY, newZ);
}

template <typename T>
T Vec3<T>::GetMagnitude() const {
	return (std::sqrt(x * x + y * y + z * z));
}

template <typename T>
T Vec3<T>::GetMagnitudeSquared() const {
	return (x * x + y * y + z * z);
}

/*
template <typename T>
T Vec3<T>::FastGetMagnitude() const {
	// look up quake stuff
	return (x * x + y * y + z * z);
}*/

template <typename T>
void Vec3<T>::ScaleX(const T scaleFactor) {
	x *= scaleFactor;
}

template <typename T>
void Vec3<T>::ScaleY(const T scaleFactor) {
	y *= scaleFactor;
}

template <typename T>
void Vec3<T>::ScaleZ(const T scaleFactor) {
	z *= scaleFactor;
}