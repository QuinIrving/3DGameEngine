#pragma once
#include <vector>
#include "Graphics/VertexIn.h"

class SphereMesh {
public:
	SphereMesh(float radius = 1.f, int numLatDivs = 24, int numLongDivs = 12);

	const std::vector<VertexIn>& GetVertices() const { return vertices; };
	const std::vector<uint32_t>& GetVertexIds() const { return vertexIds; };

private:
	int numLatDivs;
	int numLongDivs;
	float radius;

	std::vector<VertexIn> vertices;
	std::vector<uint32_t> vertexIds;
};