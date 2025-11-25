#pragma once
#include "Scene/Lights/Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight(Vec3<float> direction, float intensity = 1.f, Vec4<float> colour = {1.f, 1.f, 1.f, 1.f}) : direction(direction), Light(intensity, colour) {};

public:
	Vec3<float> direction;
};