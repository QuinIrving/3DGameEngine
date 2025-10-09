#pragma once
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Math/Vec3.h"

class Quaternion {
public:
	// we distinguish our quaternion as x,y,z,w, but our w acts like the standard (even though it's at the bottom)
	Quaternion() : delta(Vec4<float>(0.f, 0.f, 0.f, 1.f)) {};
	Quaternion(float x, float y, float z, float w) : delta(Vec4<float>(x, y, z, w).GetNormalized()) {};
	Quaternion(float real, const Vec3<float>& vector) : delta(Vec4<float>(vector.x, vector.y, vector.z, real).GetNormalized()) {};
	Quaternion(const Vec4<float>& delta) : delta(delta.GetNormalized()) {};
	Quaternion(const Vec3<float>& axis, float angle); // axis angle
	Quaternion(float rotX, float rotY, float rotZ); // euler angles
	Quaternion(const Mat4<float>& rotMat); // rot mat4

	//copy constructor
	Quaternion(const Quaternion& other) : delta(other.delta) {}

	// copy assignment
	Quaternion& operator=(const Quaternion& other);

	// move constructor
	Quaternion(Quaternion&& other) noexcept : delta(other.delta) { other.delta = Vec4<float>(); }

	// move assignment
	Quaternion& operator=(Quaternion&& other) noexcept;

	Quaternion operator~() const;
	
	// Quaternion Conjugation in place.
	Quaternion& Conjugate();

	Mat4<float> GetRotationMatrix() const;

	// Quaternion multiply (Hamilton Product)
	Quaternion operator*(const Quaternion& rhs) const; // rotation composition
	Quaternion& operator*=(const Quaternion& rhs);
	/*Quaternion operator*(const Vec4<float>& rhs) const;
	Quaternion& operator*=(const Vec4<float>& rhs);
	Quaternion operator*(const Vec3<float>& rhs) const; // transform to Vec4 with w = 0.
	Quaternion& operator*=(const Vec3<float>& rhs);*/

private:
	Vec4<float> delta; // x, y, z are vector components
};