#include "Game/Core/Events/CameraRotate.h"

Vec2<float> CameraRotate::GetFpsAngles() {
	return Vec2<float>(pitch, yaw);
}

Vec3<float> CameraRotate::GetFreecamAngles() {
	return Vec3<float>(pitch, yaw, roll);
}

float CameraRotate::GetYaw() {
	return yaw;
}

float CameraRotate::GetPitch() {
	return pitch;
}

float CameraRotate::GetRoll() {
	return roll;
}