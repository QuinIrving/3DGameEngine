#pragma once
#include "Models/SphereMesh.h"
#include "Math/Quaternion.h"

class Sphere {
public:
	Sphere(float radius = 1.f, int numLatDivs = 24, int numLongDivs = 12) : s({ radius, numLatDivs, numLongDivs }) {}

	// Want an ECS so need just making this as a placeholder for now to test my work until refactor time again
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);

	Mat4<float> GetModelMatrix();

	const std::vector<VertexIn>& GetVertices() const { return s.GetVertices(); };
	const std::vector<uint32_t>& GetVertexIds() const { return s.GetVertexIds(); };

private:
	Vec3<float> position{ 0., 0., -5. };
	Vec3<float> rotation{ 0., 0., 0. };
	Vec3<float> scale{ 1.f, 1.f, 1.f };
	Quaternion delta{};
	SphereMesh s;
};