#pragma once
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Math/Vec3.h"

class Quaternion {
public:
	// we distinguish our quaternion as x,y,z,w, but our w acts like the standard (even though it's at the bottom)
	Quaternion() : delta(Vec4<float>(0.f, 0.f, 0.f, 1.f)) {};
	Quaternion(float x, float y, float z, float w) : delta(Vec4<float>(x, y, z, w)) {};
	Quaternion(const Vec3<float>& axis, float angle); // axis angle
	Quaternion(float rotX, float rotY, float rotZ); // euler angles
	// rot mat3
	Quaternion(const Mat4<float>& rotMat); // rot mat4

	// copy constructor
	// move constructor
	//asignment constructor
	/*
	Create From axis-angle
	Create From Euler angles (pitch, yaw, roll)
	Create From rotation matrix, 3x3 or 4x4
	Copy/move constructors and assignment.
	*/


	// Quaternion Conjugation in place.
	Quaternion& operator~();

	// Quaternion multiply (Hamilton Product)
	Quaternion operator*(const Quaternion& rhs) const; // rotation composition
	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion operator*(const Vec4<float>& rhs) const;
	Quaternion& operator*=(const Vec4<float>& rhs);
	Quaternion operator*(const Vec3<float>& rhs) const; // transform to Vec4 with w = 0.
	Quaternion& operator*=(const Vec3<float>& rhs);

	Quaternion operator+(const Quaternion& rhs);
	Quaternion& operator+=(const Quaternion& rhs);
	Quaternion operator-(const Quaternion& rhs);
	Quaternion& operator-=(const Quaternion& rhs);
	Quaternion operator*(float scalar);
	Quaternion& operator*=(float scalar);
	Quaternion operator/(float scalar);
	Quaternion& operator/=(float scalar);

	bool operator==(const Quaternion& rhs) const;
	bool operator!=(const Quaternion& rhs) const;
	/*
	----
	Inverse, Inverse in place
	Get the Norm (magnitude)
	Get the length squared
	Normalize, Normalize in place
	----
	Rotate a vector
	To rotation matrix (3x3 or 4x4)
	to Axis-angle
	To Euler angles
	*/
	// dot product
	// static function: Spherical Linear interpolation (SLERP)
	// Nlerp (normalized lerp, for games)
	// LERP (faster, less accurate than SLERP)
	// Exp / Log for advanced animation/blending
	// Identity static method
	// check if normalized
	// isNearlyEqual / ApproxEqual for float tolerance checks


	/*
	QUATERNION ROTATE OPTIMIZED: (Assumes the Quaternion is normalized) else rotation won't be pure.
	Taking in a vec3 "v" can simply do
	Vec3<T> quaternion vector: qv = Vec3<T>(x, y, z);
	Vec3<T> t = 2.0f * qv.Cross(v);
	return v + w * t + qv.cross(t);

	--
	If wanting to rotate many vectors, precomputing the equivalent rot matrix from quaternion might be faster
	-
	*/

	Quaternion Conjugate() const;

private:
	Vec4<float> delta;
};

/*
-
We can override all of our operators to mainly do what we expect with Vec4 (handle it as they do)
but also override multiply for another quaternion
and add in amember function for conjugate
we can use ~ for the bitwise NOT operator. Holy.
-
*/
// trivial to find opp of a 3d rot
/*
Trivial to find min angle b/en 2 orientations,
utilizes a rot delta
No gimbal lock, interpolation is smooth and direct

4 #'s not 3.
WXYZ. 1, 0, 0, 0 = unrotated.
imagine point on circle, and then double the angle to get the value
so the point would be 0.707, 0.707 (for 90 deg)!
Which is the same as a conventional circle 45 deg.

Flipping it completely 180 degrees gives us:
(0, 1), which would be the same as 90 degrees would give us normally.

3D is a point on a 4d sphere, with interpolation between the different axes.

Why 2x angle:
to ensure there is no conflict of the 180 degrees.
and so now at the bottom of the sphere it's 360 degrees around x and around y

double-cover: 720 degree. means we can do:
find opposite of a rotation by simply making W negative.
can also flip all components and it will interpolate in other direction.

SLERP to ensure it rotates at the same speed
SQUAD is similar but for smoother like bezier. Can't be done if the points are exposed


a
*/