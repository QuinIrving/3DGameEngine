#pragma once
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Math/Quaternion.h"
#include "Math/GeneralMath.h"
#include "Math/MatrixVectorOps.h"

// potentially FOV and other stuff for perspective matrix maybe?
class Camera {
public:
	Camera() = default;

	void RotateXY(float addPitch, float addYaw);
	void Translate(float x, float y, float z);
	void FreecamTranslate(float x, float y, float z);

	Mat4<float> GetViewMatrix() const;

private:
	Quaternion rotation;
	Vec4<float> position;

	float pitch;
	float yaw;

	// freecam switch, and makes it so we don't need to worry about max rotation.

	static constexpr float MAX_PITCH = 89.f * DEGREE_TO_RADIANS;

	// use angles to update input deltas (multiplied by sensitivity),
	// if in regular camera mode (fps): clamp the pitch by 89f and -89f to be almost straight up and almost straight down.
	// then rebuild the quaterionions of qYaw, and qPitch fromaxisangle, and then multiply them to get cameraRot;
};