#pragma once
#include "Models/CubeMesh.h"
#include "Math/MatrixVectorOps.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include "Math/Quaternion.h"
#include "Graphics/ModelAttributes.h"

class Cube {
public:
	Cube() = default;
	Cube(float size);

	// Want an ECS so need just making this as a placeholder for now to test my work until refactor time again
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);

	ModelAttributes GetModelAttributes();
	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

	Material material; // not a pointer, will refactor later when we want to do sharing.
private:
	Vec3<float> position{ 0., 0., 0. };
	Vec3<float> rotation{ 0., 0., 0. };
	Vec3<float> scale{ .5, .5, .5 };
	Quaternion delta;
	CubeMesh cube;
};