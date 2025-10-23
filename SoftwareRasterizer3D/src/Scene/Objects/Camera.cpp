#include "Scene/Objects/Camera.h"

Mat4<float> Camera::GetViewMatrix() const {
	Quaternion qYaw = Quaternion(Vec3<float>(0.f, 1.f, 0.f), yaw);
	Quaternion qPitch = Quaternion(Vec3<float>(1.f, 0.f, 0.f), pitch);
	Quaternion orientation = qYaw * qPitch;

	Mat4<float> m = orientation.GetRotationMatrix();
	Vec4<float> newPos = -(position * m);

	// set the translation of the camera to the correct locations;
	m[3][0] = newPos.x;
	m[3][1] = newPos.y;
	m[3][2] = newPos.z;

	return m;
}

void Camera::RotateXY(float addPitch, float addYaw) {
	pitch += addPitch;
	pitch = std::clamp(pitch, -MAX_PITCH, MAX_PITCH);
	
	yaw = fmodf(yaw + addYaw, 2 * PI);
}

void Camera::Translate(float x, float y, float z) {
	Vec3<float> forward = Vec3<float>(0.f, 0.f, 1.f);
	const Quaternion yawQuat = Quaternion(Vec3<float>(0.f, 1.f, 0.f), yaw);
	forward *= yawQuat;

	Vec3<float> right = Vec3<float>(1.f, 0.f, 0.f);
	right *= yawQuat;

	forward *= z;
	right *= x;

	position += forward;
	position += right;
}

void Camera::Translate(const Vec3<float>& v) {
	Translate(v.x, v.y, v.z);
}

void Camera::FreecamTranslate(float x, float y, float z) {
	Vec3<float> tPos = Vec3<float>(x, y, z) * Quaternion(pitch, yaw, 0.f).Normalize();

	position.x += tPos.x;
	position.y += tPos.y;
	position.z += tPos.z;
}

const Vec4<float>& Camera::GetPosition() const {
	return position;
}