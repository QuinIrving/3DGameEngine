#pragma once
#include "Math/Mat4.h"
#include "Math/MatrixVectorOps.h"
#include "Math/Vec4.h"
#include "Math/Quaternion.h"
#include "Math/GeneralMath.h"
// View matrix - keep it normal, then a function to get the inverse once a frame.
// only rotation and translation needed for a camera.
// potentially FOV and other stuff for perspective matrix maybe?
class Camera {
public:
	Camera() = default;

	void RotateXY(float rotX, float rotY);
	void Translate(float x, float y, float z);

	Mat4<float> GetViewMatrix() const;

private:
	Quaternion rotation;
	Vec4<float> position;
};