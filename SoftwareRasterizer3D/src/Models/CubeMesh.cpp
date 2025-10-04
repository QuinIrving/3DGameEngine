#include "Models/CubeMesh.h"

// Look into std::span for using std::array instead of vertex. See if it will help with efficiency.
CubeMesh::CubeMesh() {
	vertices.reserve(8);
	vertexIds.reserve(36);

	float size = 1.f;
	vertices.push_back({-size, size, size});       // 0
	vertices.push_back({ size, size, size });      // 1
	vertices.push_back({ -size,  -size, size });   // 2
	vertices.push_back({ size,  -size, size });    // 3
	vertices.push_back({ -size, size,  -size });   // 4
	vertices.push_back({ size, size,  -size });    // 5
	vertices.push_back({ -size,  -size,  -size }); // 6
	vertices.push_back({ size,  -size,  -size });  // 7
	
	//Front Face 1:
	vertexIds.push_back(0);
	vertexIds.push_back(2);
	vertexIds.push_back(3);

	// Front Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(3);
	vertexIds.push_back(1);

	//Back Face 1:
	vertexIds.push_back(4);
	vertexIds.push_back(6);
	vertexIds.push_back(7);

	// Back Face 2:
	vertexIds.push_back(4);
	vertexIds.push_back(7);
	vertexIds.push_back(5);

	// Right Face 1:
	vertexIds.push_back(1);
	vertexIds.push_back(3);
	vertexIds.push_back(7);

	// Right Face 2:
	vertexIds.push_back(1);
	vertexIds.push_back(7);
	vertexIds.push_back(5);

	// Left Face 1:
	vertexIds.push_back(0);
	vertexIds.push_back(2);
	vertexIds.push_back(6);

	// Left Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(6);
	vertexIds.push_back(4);

	// Bottom Face 1:
	vertexIds.push_back(0);
	vertexIds.push_back(4);
	vertexIds.push_back(5);

	// Bottom Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(5);
	vertexIds.push_back(1);

	// Top Face 1:
	vertexIds.push_back(2);
	vertexIds.push_back(6);
	vertexIds.push_back(7);

	// Top Face 2:
	vertexIds.push_back(2);
	vertexIds.push_back(7);
	vertexIds.push_back(3);
}

std::vector<uint32_t>& CubeMesh::GetVertexIds() {
	return vertexIds;
}

std::vector<VertexIn>& CubeMesh::GetVertices() {
	return vertices;
}
