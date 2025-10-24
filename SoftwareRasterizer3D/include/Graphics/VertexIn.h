#pragma once
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/MatrixVectorOps.h"

class VertexIn {
public:
	VertexIn() = default;
	VertexIn(Vec3<float> pos) : position(Vec3<float>(pos)) {}
	VertexIn(Vec4<float> pos) : position(Vec3<float>(pos.x, pos.y, pos.z)) {}
	VertexIn(float x, float y, float z) : position(Vec3<float>(x, y, z)) {}

	Vec4<float> operator*(const Mat4<float>& rhs) const;

	const Vec3<float>& GetPosition() const;
	const Vec4<float>& GetColour() const;
	const Vec3<float>& GetNormal() const;
	const Vec2<float>& GetUV() const;

	void SetNormal(Vec3<float> norm);
	void SetColour(int r, int g, int b, int a = 255);

private:
	Vec3<float> position;
	Vec4<float> colour;
	Vec3<float> normal;
	Vec2<float> UV = { 0, 0 };
	// 	e.g., tangent, binormal, bone weights, etc.
};

//3d pos is (x, y, z, 1)
//3d dir is (x, y, z, 0)