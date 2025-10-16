#include "Scene/Objects/Camera.h"

// identity matrix until camera is implemented, then we provide the inverse of the transformations applied to this camera.
// I believe it's a look at function.
Mat4<float> Camera::GetViewMatrix() const {
	Mat4<float> m = rotation.GetRotationMatrix().GetTranspose();
	Vec4<float> newPos = -(position * m);

	// set the translation of the camera to the correct locations;
	m[3][0] = newPos.x;
	m[3][1] = newPos.y;
	m[3][2] = newPos.z;

	return m;
}

void Camera::RotateXY(float rotX, float rotY) {
	// inverse it.
	Quaternion q = Quaternion(rotX, rotY, 0.f);
	rotation *= q;
	rotation.Normalize();
}

void Camera::Translate(float x, float y, float z) {
	Vec3<float> forward = Vec3<float>(0.f, 0.f, 1.f) * rotation;
	forward.y = 0.0f;
	forward.Normalize();

	float yaw = atan2f(forward.x, forward.z);
	Quaternion qY = Quaternion(Vec3<float>(0.f, 1.f, 0.f), yaw);
	
	Vec3<float> tPos = Vec3<float>(x, y, z) * qY;
	// inverse it
	position.x += tPos.x;
	//position.y += tPos.y;
	position.z += tPos.z;
}

void Camera::FreecamTranslate(float x, float y, float z) {
	Vec3<float> tPos = Vec3<float>(x, y, z) * rotation;
	// inverse it
	position.x += tPos.x;
	position.y += tPos.y;
	position.z += tPos.z;
}

// void Camera::FreecamTranslate()....
// Vec3<float> tPos = Vec3<float>(x, y, z) * rotation; // should be our regular 'free-cam' mode of translation