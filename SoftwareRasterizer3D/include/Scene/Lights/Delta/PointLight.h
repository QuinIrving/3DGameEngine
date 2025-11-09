#pragma once
#include "Scene/Lights/Light.h"

class PointLight : public Light {
public:
	PointLight(Vec3<float> position) : position(position) {};
	
public:
	Vec3<float> position;
};