#pragma once
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/MatrixVectorOps.h"
#include "VertexPostClip.h"

class VertexOut {
public:
	enum class TestPlane {
		LEFT, // x >= -w in plane
		RIGHT, // x <= w in plane
		TOP, // y >= -w in plane
		BOTTOM, // y <= w in plane
		FRONT, // z >= 0 in plane
		BACK // z <= w in plane
	};

public:
	VertexOut() = default;
	VertexOut(Vec4<float> pos) : position(pos) {}
	//VertexOut(Vec3<float> pos) : position(Vec3<float>(pos.x, pos.y, pos.z)) {}
	VertexOut(float x, float y, float z, float w) : position(Vec4<float>(x, y, z, w)) {}
	VertexOut(Vec4<float> pos, Vec3<float> worldPos, Vec3<float> viewPos, Vec3<float> normal, Vec2<float> uv) 
		: position(pos), worldPosition(worldPos), viewPosition(viewPos), normal(normal), UV(uv) {};
	VertexOut(Vec4<float> pos, Vec3<float> worldPos, Vec3<float> viewPos, Vec3<float> normal, Vec2<float> uv, Vec3<float> tangent, Vec3<float> bitangent) 
		: position(pos), worldPosition(worldPos), viewPosition(viewPos), normal(normal), UV(uv), tangent(tangent), bitangent(bitangent) {};

	Vec4<float> operator*(const Mat4<float>& rhs) const;

	const Vec4<float>& GetPosition() const;
	const Vec3<float>& GetWorldPosition() const;
	const Vec3<float>& GetViewPosition() const;
	const Vec4<float>& GetColour() const;
	const Vec3<float>& GetNormal() const;
	const Vec2<float>& GetUV() const;
	const Vec3<float>& GetTangent() const;
	const Vec3<float>& GetBitangent() const;

	//const Vec3<float>& CalculateNormal();
	void SetColour(int r, int g, int b, int a = 255);


	static bool IsTriangleInFrustum(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	bool IsInFrustumPlane(TestPlane plane) const;
	bool IsNotInNearFrustum() const;
	VertexPostClip PerspectiveDivide() const;

private:
	Vec4<float> position;
	Vec3<float> worldPosition{}; // vert in's multiplied only by M, for normal calculations during triangle assembly lol.
	Vec3<float> viewPosition{}; // vertex in's multiplied only by MV, for normal calculations during triangle assembly.
	Vec4<float> colour;
	Vec3<float> normal;
	Vec2<float> UV = { 0, 0 };
	Vec3<float> tangent = { 0, 0, 0 };
	Vec3<float> bitangent = { 0, 0, 0 };
	//  bone weights, etc.
};

//3d pos is (x, y, z, 1)
//3d dir is (x, y, z, 0)