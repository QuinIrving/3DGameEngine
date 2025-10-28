#pragma once

struct Colour {
	Colour() : r(0.f), g(0.f), b(0.f), a(1.f) {}
	Colour(float r, float g, float b, float a = 1.f) : r(r), g(g), b(b), a(a) {}

	float r;
	float g;
	float b;
	float a;
};