#pragma once
#include <vector>
#include "Models/Vertex.h"

// a Cube can have 8 vertices only needed if it shares faces
class CubeMesh {
public:
	CubeMesh();
	
	std::vector<int>& GetVertexIds();
	std::vector<Vertex>& GetVertices();

private:
	std::vector<Vertex> vertices;
	std::vector<int> vertexIds;
};