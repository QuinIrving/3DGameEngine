#pragma once
#include "GameEvent.h"
#include <utility>

class CameraRotate : public GameEvent {
public:
	CameraRotate(float yaw, float pitch) : GameEvent(GameEvent::Type::CameraRotate), yaw(yaw), pitch(pitch) {};

	std::pair<float, float> GetAngles();
	float GetYaw();
	float GetPitch();

private:
	float yaw;
	float pitch;
};