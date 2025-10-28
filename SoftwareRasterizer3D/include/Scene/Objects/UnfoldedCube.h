#pragma once
#include "Models/UnfoldedCubeMesh.h"
#include "Math/MatrixVectorOps.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include "Math/Quaternion.h"

class UnfoldedCube {
public:
	UnfoldedCube() = default;
	UnfoldedCube(float size);

	// Want an ECS so need just making this as a placeholder for now to test my work until refactor time again
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);

	Mat4<float> GetModelMatrix();
	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

private:
	Vec3<float> position{ 0., 0., 0. };
	Vec3<float> rotation{ 0., 0., 0. };
	Vec3<float> scale{ .5, .5, .5 };
	Quaternion delta;
	UnfoldedCubeMesh cube;
};