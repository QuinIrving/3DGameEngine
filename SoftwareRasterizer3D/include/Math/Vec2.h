#pragma once
#include <cmath>

template <typename T>
class Vec2 {
public:
	constexpr Vec2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {};
	constexpr Vec2(T x, T y) : x(x), y(y) {};

	// Vector addition & subtraction
	Vec2<T> operator+(const Vec2<T>& rhs) const;
	Vec2<T> operator-(const Vec2<T>& rhs) const;
	Vec2<T>& operator+=(const Vec2<T>& rhs);
	Vec2<T>& operator-=(const Vec2<T>& rhs);

	// Scalar vector multiplication (vector scaling)
	Vec2<T> operator*(const T scalar) const;
	Vec2<T> operator/(const T scalar) const;
	Vec2<T>& operator*=(const T scalar);
	Vec2<T>& operator/=(const T scalar);

	// Equality, check if all element's are equal
	bool operator==(const Vec2<T>& rhs) const;
	bool operator!=(const Vec2<T>& rhs) const;

	constexpr T DotProduct(const Vec2<T>& rhs) const;
	constexpr static T DotProduct(const Vec2<T>& lhs, const Vec2<T>& rhs);

	Vec2<T> GetNormalized() const;
	T GetMagnitude() const;
	T GetMagnitudeSquared() const;

	void ScaleX(const T scaleFactor);
	void ScaleY(const T scaleFactor);
	void ScaleZ(const T scaleFactor);
	// T FastGetMagnitude() const; // probably something with the quake idea. Inverse square root approximation.

public:
	T x;
	T y;
};

template <typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const {
	T newX = x + rhs.x;
	T newY = y + rhs.y;

	return Vec2<T>(newX, newY);
}

template <typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const {
	T newX = x - rhs.x;
	T newY = y - rhs.y;

	return Vec2<T>(newX, newY);
}

template <typename T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& rhs) {
	x += rhs.x;
	y += rhs.y;

	return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

template <typename T>
Vec2<T> Vec2<T>::operator*(const T scalar) const {
	T newX = x * scalar;
	T newY = y * scalar;

	return Vec2<T>(newX, newY);
}

template <typename T>
Vec2<T> Vec2<T>::operator/(const T scalar) const {
	if (scalar == static_cast<T>(0)) {
		// Output some warning
		return Vec2<T>();
	}

	T newX = x / scalar;
	T newY = y / scalar;

	return Vec2<T>(newX, newY);
}

template <typename T>
Vec2<T>& Vec2<T>::operator*=(const T scalar) {
	x *= scalar;
	y *= scalar;

	return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator/=(const T scalar) {
	if (scalar == static_cast<T>(0)) {
		// Output some warning or probably throw an exception.
		return *this;
	}

	x /= scalar;
	y /= scalar;

	return *this;
}

template <typename T>
bool Vec2<T>::operator==(const Vec2<T>& rhs) const {
	return x == rhs.x && y == rhs.y;
}

template <typename T>
bool Vec2<T>::operator!=(const Vec2<T>& rhs) const {
	return !(*this == rhs);
}

template<typename T>
inline constexpr T Vec2<T>::DotProduct(const Vec2<T>& rhs) const
{
	return x * rhs.x + y * rhs.y;
}

template<typename T>
inline constexpr T Vec2<T>::DotProduct(const Vec2<T>& lhs, const Vec2<T>& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T>
Vec2<T> Vec2<T>::GetNormalized() const {
	T mag = this->GetMagnitude();

	if (mag == 0) {
		// probably should do some exception here.
		return Vec2<T>();
	}

	T newX = x / mag;
	T newY = y / mag;

	return Vec2<T>(newX, newY);
}

template <typename T>
T Vec2<T>::GetMagnitude() const {
	return (std::sqrt(x * x + y * y));
}

template <typename T>
T Vec2<T>::GetMagnitudeSquared() const {
	return (x * x + y * y);
}

/*
template <typename T>
T Vec2<T>::FastGetMagnitude() const {
	// look up quake stuff
	return (x * x + y * y);
}*/

template <typename T>
void Vec2<T>::ScaleX(const T scaleFactor) {
	x *= scaleFactor;
}

template <typename T>
void Vec2<T>::ScaleY(const T scaleFactor) {
	y *= scaleFactor;
}