#pragma once
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"

class Vertex {
public:
	Vertex() = default;
	Vertex(Vec3<int> pos) : position(Vec3<int>(pos)) {}
	Vertex(int x, int y, int z) : position(Vec3<int>(x, y, z)) {}

	Vec3<int>& GetPosition();
	Vec4<int>& GetColour();
	Vec3<float>& GetNormal();
	Vec2<float>& GetUV();

	void CalculateNormal();
	void SetColour(int r, int g, int b, int a = 255);

private:
	Vec3<int> position;
	Vec4<int> colour;
	Vec3<float> normal;
	Vec2<float> UV;
};

//3d pos is (x, y, z, 1)
//3d dir is (x, y, z, 0)