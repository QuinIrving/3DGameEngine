#include "Math/Quaternion.h"

// we distinguish our quaternion as x,y,z,w, but our w acts like the standard (even though it's at the bottom)
Quaternion::Quaternion(const Vec3<float>& axis, float angle) { // axis angle
	Vec3<float> axisNormalized = axis.GetNormalized();
	float s = sinf(angle * 0.5f);

	delta = Vec4<float>();
	delta.x = axisNormalized.x * s;
	delta.y = axisNormalized.y * s;
	delta.z = axisNormalized.z * s;
	delta.w = cosf(angle * 0.5f);
}
Quaternion::Quaternion(float rotX, float rotY, float rotZ) { // euler angles
	// q = qx * qy * qz.
	// optimizing:
	float halfX = rotX * 0.5f;
	float halfY = rotY * 0.5f;
	float halfZ = rotZ * 0.5f;

	float cosX = cosf(halfX);
	float cosY = cosf(halfY);
	float cosZ = cosf(halfZ);
	
	float sinX = sinf(halfX);
	float sinY = sinf(halfY);
	float sinZ = sinf(halfZ);

	float x = (sinX * cosY * cosZ) - (cosX * sinY * sinZ);
	float y = (cosX * sinY * cosZ) + (sinX * cosY * sinZ);
	float z = (cosX * cosY * sinZ) - (sinX * sinY * cosZ);
	float w = (cosX * cosY * cosZ) + (sinX * sinY * sinZ);

	delta = Vec4<float>(x, y, z, w);
}

Quaternion::Quaternion(const Mat4<float>& rotMat) { // rot mat4
	// bypass for now.
}

Quaternion& Quaternion::operator=(const Quaternion& other) {
	if (this != &other) {
		delta = other.delta;
	}

	return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& other) noexcept {
	if (this != &other) {
		delta = other.delta;
		other.delta = Vec4<float>();
	}
	return *this;
}

Quaternion Quaternion::operator~() const {
	return Quaternion(-delta.x, -delta.y, -delta.z, delta.w);
}

// Quaternion Conjugation in place.
Quaternion& Quaternion::Conjugate() {
	delta.x = -delta.x;
	delta.y = -delta.y;
	delta.z = -delta.z;

	return *this;
}


// Quaternion multiply (Hamilton Product)
Quaternion Quaternion::operator*(const Quaternion& rhs) const { // rotation composition
	Vec4<float> result;
	result.x = (delta.w * rhs.delta.x) + (rhs.delta.w * delta.x) + (delta.y * rhs.delta.z) - (delta.z * rhs.delta.y);
	result.y = (delta.w * rhs.delta.y) + (rhs.delta.w * delta.y) + (delta.z * rhs.delta.x) - (delta.x * rhs.delta.z);
	result.z = (delta.w * rhs.delta.z) + (rhs.delta.w * delta.z) + (delta.x * rhs.delta.y) - (delta.y * rhs.delta.x);
	result.w = (delta.w * rhs.delta.w) - (delta.x * rhs.delta.x) - (delta.y * rhs.delta.y) - (delta.z * rhs.delta.z);

	return Quaternion(result);
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
	Vec4<float> result;
	result.x = (delta.w * rhs.delta.x) + (rhs.delta.w * delta.x) + (delta.y * rhs.delta.z) - (delta.z * rhs.delta.y);
	result.y = (delta.w * rhs.delta.y) + (rhs.delta.w * delta.y) + (delta.z * rhs.delta.x) - (delta.x * rhs.delta.z);
	result.z = (delta.w * rhs.delta.z) + (rhs.delta.w * delta.z) + (delta.x * rhs.delta.y) - (delta.y * rhs.delta.x);
	result.w = (delta.w * rhs.delta.w) - (delta.x * rhs.delta.x) - (delta.y * rhs.delta.y) - (delta.z * rhs.delta.z);

	delta = result;
	return *this;
}

Mat4<float> Quaternion::GetRotationMatrix() const {
	Mat4<float> m = Mat4<float>::GetIdentity();
	
	m[0][0] = 2 * (delta.w * delta.w + delta.x * delta.x) - 1;
	m[0][1] = 2 * (delta.x * delta.y - delta.w * delta.z);
	m[0][2] = 2 * (delta.x * delta.z + delta.w * delta.y);

	m[1][0] = 2 * (delta.x * delta.y + delta.w * delta.z);
	m[1][1] = 2 * (delta.w * delta.w + delta.y * delta.y) - 1;
	m[1][2] = 2 * (delta.y * delta.z - delta.w * delta.x);

	m[2][0] = 2 * (delta.x * delta.z - delta.w * delta.y);
	m[2][1] = 2 * (delta.y * delta.z + delta.w * delta.x);
	m[2][2] = 2 * (delta.w * delta.w + delta.z * delta.z) - 1;

	/*
	float x = delta.x, y = delta.y, z = delta.z, w = delta.w;
	float xx = x * x, yy = y * y, zz = z * z;
	float xy = x * y, xz = x * z, yz = y * z;
	float wx = w * x, wy = w * y, wz = w * z;

	Mat4<float> m = Mat4<float>::GetIdentity();

	m[0][0] = 1.0f - 2.0f * (yy + zz);
	m[0][1] = 2.0f * (xy + wz);
	m[0][2] = 2.0f * (xz - wy);

	m[1][0] = 2.0f * (xy - wz);
	m[1][1] = 1.0f - 2.0f * (xx + zz);
	m[1][2] = 2.0f * (yz + wx);

	m[2][0] = 2.0f * (xz + wy);
	m[2][1] = 2.0f * (yz - wx);
	m[2][2] = 1.0f - 2.0f * (xx + yy);
	*/
	return m;
}

Quaternion& Quaternion::Normalize() {
	delta = delta.GetNormalized();

	return *this;
}

/* Probably actually need it a lil different.
Quaternion Quaternion::operator*(const Vec4<float>& rhs) const {
	
}

Quaternion& Quaternion::operator*=(const Vec4<float>& rhs) {

}

Quaternion Quaternion::operator*(const Vec3<float>& rhs) const { // transform to Vec4 with w = 0.

}

Quaternion& Quaternion::operator*=(const Vec3<float>& rhs) {

	return *this;
}*/
