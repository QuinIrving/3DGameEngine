#pragma once
#include "Math/Vec3.h"
#include "Math/Vec2.h"
#include "GameEvent.h"
#include <utility>

class CameraRotate : public GameEvent {
public:
	CameraRotate(float pitch, float yaw) : GameEvent(GameEvent::Type::CameraRotate), pitch(pitch), yaw(yaw), roll(0.f) {};
	CameraRotate(float pitch, float yaw, float roll) : GameEvent(GameEvent::Type::CameraRotate), pitch(pitch), yaw(yaw), roll(0.f) {};

	Vec2<float> GetFpsAngles();
	Vec3<float> GetFreecamAngles();

	float GetPitch();
	float GetYaw();
	float GetRoll();

private:
	float pitch;
	float yaw;
	float roll;
};