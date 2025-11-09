#pragma once
#include "Math/Vec4.h"
class Light {
public:
	virtual ~Light() = default;
	Vec4<float> colour{ 1.f, 1.f, 1.f, 1.f }; // maybe vec3
	float intensity{ 1.f };
	// virtual Vec4<float> sample(...)
};