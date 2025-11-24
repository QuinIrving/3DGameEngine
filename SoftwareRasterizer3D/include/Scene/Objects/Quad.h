#pragma once
#include "Math/MatrixVectorOps.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include "Math/Quaternion.h"
#include "Graphics/ModelAttributes.h"
#include <Graphics/VertexIn.h>
#include <Models/QuadMesh.h>

class Quad {
public:
	Quad() = default;
	Quad(float width=1.f, float height=1.f, float size = 1.f);
	// Want an ECS so need just making this as a placeholder for now to test my work until refactor time again
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);

	ModelAttributes GetModelAttributes();
	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

	Material material;
private:
	Vec3<float> position{ 0., 0., 0. };
	Vec3<float> rotation{ 0., 0., 0. };
	Vec3<float> scale{ .5, .5, .5 };
	Quaternion delta;
	QuadMesh mesh;
};