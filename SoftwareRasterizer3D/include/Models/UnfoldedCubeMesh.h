#pragma once
#include <vector>
#include "Graphics/VertexIn.h"

// 24 vertices. None shared. The vertex normals are simply the normals of the face the vertex contains.
class UnfoldedCubeMesh {
public:
	UnfoldedCubeMesh();

	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

	void SetVertexNormals();

private:
	std::vector<VertexIn> vertices;
	std::vector<uint32_t> vertexIds;
};