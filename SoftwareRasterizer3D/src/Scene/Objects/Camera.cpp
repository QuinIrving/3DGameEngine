#include "Scene/Objects/Camera.h"

// identity matrix until camera is implemented, then we provide the inverse of the transformations applied to this camera.
// I believe it's a look at function.
Mat4<float> Camera::GetViewMatrix() const {
	return Mat4<float>();
}