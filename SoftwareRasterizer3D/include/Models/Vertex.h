#pragma once
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"

class Vertex {
public:
	
private:
	Vec3<float> position;
	Vec4<float> colour;
	Vec3<float> normal;
	Vec2<float> UV;
};

//3d pos is (x, y, z, 1)
//3d dir is (x, y, z, 0)