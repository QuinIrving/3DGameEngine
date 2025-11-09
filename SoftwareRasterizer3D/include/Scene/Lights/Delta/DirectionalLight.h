#pragma once
#include "Scene/Lights/Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight(Vec3<float> direction) : direction(direction) {};

public:
	Vec3<float> direction;
};