#include "Game/Core/Events/CameraRotate.h"

std::pair<float, float> CameraRotate::GetAngles() {
	return std::pair<float, float>(yaw, pitch);
}

float CameraRotate::GetYaw() {
	return yaw;
}

float CameraRotate::GetPitch() {
	return pitch;
}