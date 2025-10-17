#include "Scene/Objects/Camera.h"

// identity matrix until camera is implemented, then we provide the inverse of the transformations applied to this camera.
// I believe it's a look at function.
Mat4<float> Camera::GetViewMatrix() const {
	Quaternion qYaw = Quaternion(Vec3<float>(0.f, 1.f, 0.f), yaw);
	Quaternion qPitch = Quaternion(Vec3<float>(1.f, 0.f, 0.f), pitch);
	Quaternion orientation = qYaw * qPitch;
	//Mat4<float> m = Quaternion(pitch, yaw, 0.f).GetRotationMatrix().GetTranspose();
	Mat4<float> m = orientation.GetRotationMatrix();
	Vec4<float> newPos = -(position * m);

	// set the translation of the camera to the correct locations;
	m[3][0] = newPos.x;
	m[3][1] = newPos.y;
	m[3][2] = newPos.z;

	return m;
}

void Camera::RotateXY(float addPitch, float addYaw) {
	// inverse it.
	/*Quaternion q = Quaternion(rotX, rotY, 0.f);
	rotation *= q;
	rotation.Normalize();*/
	pitch += addPitch;
	pitch = std::clamp(pitch, -MAX_PITCH, MAX_PITCH);
	
	yaw = fmodf(yaw + addYaw, 2 * PI);
}

void Camera::Translate(float x, float y, float z) {
	/*Vec3<float> forward = Vec3<float>(0.f, 0.f, 1.f) * rotation;
	forward.y = 0.0f;
	forward.Normalize();

	float yaw = atan2f(forward.x, forward.z);
	Quaternion qY = Quaternion(Vec3<float>(0.f, 1.f, 0.f), yaw);*/
	
	/*Vec3<float> tPos = Vec3<float>(x, 0.f, z) * Quaternion(pitch, yaw, 0.f).Normalize();
	position.x += tPos.x;
	//position.y += tPos.y;
	position.z += tPos.z;*/

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

void Camera::FreecamTranslate(float x, float y, float z) {
	Vec3<float> tPos = Vec3<float>(x, y, z) * Quaternion(pitch, yaw, 0.f).Normalize();
	// inverse it
	position.x += tPos.x;
	position.y += tPos.y;
	position.z += tPos.z;
}

// void Camera::FreecamTranslate()....
// Vec3<float> tPos = Vec3<float>(x, y, z) * rotation; // should be our regular 'free-cam' mode of translation