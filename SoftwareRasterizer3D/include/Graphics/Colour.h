#pragma once
#include "Math/Vec4.h"

struct Colour {
	Colour() : r(1.f), g(1.f), b(1.f), a(1.f) {}
	Colour(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {};
	Colour(Vec4<float> v) : r(v.x), g(v.y), b(v.z), a(v.w) {};

	Colour operator*(float scale) {
		return Colour(r * scale, g * scale, b * scale, a);
	}

	Colour& operator*=(float scale) {
		r *= scale;
		g *= scale;
		b *= scale;

		return *this;
	}

	float r;
	float g;
	float b;
	float a;
};