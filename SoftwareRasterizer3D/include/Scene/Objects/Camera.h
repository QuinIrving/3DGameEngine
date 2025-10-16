#pragma once
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Math/Quaternion.h"
#include "Math/GeneralMath.h"
#include "Math/MatrixVectorOps.h"
// View matrix - keep it normal, then a function to get the inverse once a frame.
// only rotation and translation needed for a camera.
// potentially FOV and other stuff for perspective matrix maybe?
class Camera {
public:
	Camera() = default;

	void RotateXY(float rotX, float rotY);
	void Translate(float x, float y, float z);
	void FreecamTranslate(float x, float y, float z);

	Mat4<float> GetViewMatrix() const;

private:
	Quaternion rotation;
	Vec4<float> position;

	float yaw;
	float pitch;

	// use angles to update input deltas (multiplied by sensitivity),
	// if in regular camera mode (fps): clamp the pitch by 89f and -89f to be almost straight up and almost straight down.
	// then rebuild the quaterionions of qYaw, and qPitch fromaxisangle, and then multiply them to get cameraRot;
};