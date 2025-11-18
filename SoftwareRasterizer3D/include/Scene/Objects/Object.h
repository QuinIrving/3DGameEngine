#pragma once
#include "Models/Mesh.h"
#include "Math/MatrixVectorOps.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include "Math/Quaternion.h"
#include "Graphics/ModelAttributes.h"
#include "Graphics/VertexIn.h"

class Object {
public:
	Object() = default;
	Object(float size, std::vector<VertexIn> vertices, std::vector<uint32_t> vertexIds);

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
	Vec3<float> scale{ 1, 1, 1 };
	Quaternion delta;
	Mesh mesh;
};