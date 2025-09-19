#pragma once
#include <array>
#include "Vertex.h"

// a Cube can have 8 vertices only needed if it shares faces
class Cube {
public:
	Cube();
	Cube(float size);
	
	std::array<int, 36>& GetVertexIds();
	std::array<Vertex, 8>& GetVertices();

private:
	std::array<Vertex, 8> vertices;
	std::array<int, 36> vertexIds;
};