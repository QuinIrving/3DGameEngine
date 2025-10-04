#pragma once
#include "Math/Mat4.h"
// View matrix - keep it normal, then a function to get the inverse once a frame.
// only rotation and translation needed for a camera.
// potentially FOV and other stuff for perspective matrix maybe?
class Camera {
public:
	Camera() = default;

	Mat4<float> GetViewMatrix() const;

private:

};