#pragma once
#include <vector>
#include "Graphics/VertexIn.h"
// Should be our generic mesh, so it can also be used by obj loaders. May need it to include normals? idk, look into a generic Object then as well?

class Mesh {
public:
	Mesh() = default;
	Mesh(std::vector<VertexIn> vertices, std::vector<uint32_t> vertexIds) : vertices(vertices), vertexIds(vertexIds) {};

	std::vector<uint32_t>& GetVertexIds() { return vertexIds; }
	std::vector<VertexIn>& GetVertices() { return vertices; }

	void GenerateVertexNormals() {}; // should change this to generate if necessary some vertex normals via the face generation incase we need.
private:
	std::vector<VertexIn> vertices;
	std::vector<uint32_t> vertexIds;
};