#pragma once
#include "Math/Vec4.h"

struct FragmentOut {
public:
	FragmentOut() = default;
	FragmentOut(float r, float g, float b, float a = 255.f) : colour({ r, g, b, a }) {}
	Vec4<float> colour;
};