#pragma once
#include <vector>
#include "Graphics/VertexIn.h"

// a Cube can have 8 vertices only needed if it shares faces
class CubeMesh {
public:
	CubeMesh();
	
	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

	void SetVertexNormals();

private:
	std::vector<VertexIn> vertices;
	std::vector<uint32_t> vertexIds;
};