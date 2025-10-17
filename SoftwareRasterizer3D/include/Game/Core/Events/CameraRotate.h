#pragma once
#include "GameEvent.h"
#include <utility>

class CameraRotate : public GameEvent {
public:
	CameraRotate(float pitch, float yaw) : GameEvent(GameEvent::Type::CameraRotate), pitch(pitch), yaw(yaw) {};

	std::pair<float, float> GetAngles();
	float GetYaw();
	float GetPitch();

private:
	float pitch;
	float yaw;
};