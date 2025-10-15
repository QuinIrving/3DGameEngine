#include "Scene/Objects/Camera.h"

// identity matrix until camera is implemented, then we provide the inverse of the transformations applied to this camera.
// I believe it's a look at function.
Mat4<float> Camera::GetViewMatrix() const {
	Mat4<float> m = rotation.GetRotationMatrix();
	// set the translation of the camera to the correct locations;
	m[3][0] = position.x;
	m[3][1] = position.y;
	m[3][2] = position.z;

	return m;
}

void Camera::RotateXY(float rotX, float rotY) {
	// inverse it.
	Quaternion q = Quaternion(-rotX, -rotY, 0.f);
	rotation *= q;
}

void Camera::Translate(float x, float y, float z) {
	// inverse it
	position.x -= x;
	position.y -= y;
	position.z -= z;
}