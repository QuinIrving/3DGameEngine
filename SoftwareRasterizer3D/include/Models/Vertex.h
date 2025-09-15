#pragma once
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"

class Vertex {
public:
	Vertex() = default;
	Vertex(Vec3<float> pos) : position(Vec3<float>(pos)) {}
	Vertex(float x, float y, float z) : position(Vec3<float>(x, y, z)) {}

	Vec3<float>& GetPosition();
	Vec4<int>& GetColour();
	Vec3<float>& GetNormal();
	Vec2<float>& GetUV();

	Vec3<float>& CalculateNormal();
	void SetColour(int r, int g, int b, int a = 255);

private:
	Vec3<float> position;
	Vec4<int> colour;
	Vec3<float> normal;
	Vec2<float> UV;
};

//3d pos is (x, y, z, 1)
//3d dir is (x, y, z, 0)