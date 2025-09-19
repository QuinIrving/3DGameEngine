#include "Models/Cube.h"

Cube::Cube() : Cube(1.0) {}

Cube::Cube(float size) {
	vertices[0] = { -size, size, size };
	vertices[1] = { size, size, size };
	vertices[2] = { -size,  -size, size };
	vertices[3] = { size,  -size, size };
	vertices[4] = { -size, size,  -size };
	vertices[5] = { size, size,  -size };
	vertices[6] = { -size,  -size,  -size };
	vertices[7] = { size,  -size,  -size };

	//Front Face 1:
	vertexIds[0] = 0;
	vertexIds[1] = 2;
	vertexIds[2] = 3;

	// Front Face 2:
	vertexIds[3] = 0;
	vertexIds[4] = 3;
	vertexIds[5] = 1;

	//Back Face 1:
	vertexIds[6] = 4;
	vertexIds[7] = 6;
	vertexIds[8] = 7;

	// Back Face 2:
	vertexIds[9] = 4;
	vertexIds[10] = 7;
	vertexIds[11] = 5;

	// Right Face 1:
	vertexIds[12] = 1;
	vertexIds[13] = 3;
	vertexIds[14] = 7;

	// Right Face 2:
	vertexIds[15] = 1;
	vertexIds[16] = 7;
	vertexIds[17] = 5;

	// Left Face 1:
	vertexIds[18] = 0;
	vertexIds[19] = 2;
	vertexIds[20] = 6;

	// Left Face 2:
	vertexIds[21] = 0;
	vertexIds[22] = 6;
	vertexIds[23] = 4;

	// Bottom Face 1:
	vertexIds[24] = 0;
	vertexIds[25] = 4;
	vertexIds[26] = 5;

	// Bottom Face 2:
	vertexIds[27] = 0;
	vertexIds[28] = 5;
	vertexIds[29] = 1;

	// Top Face 1:
	vertexIds[30] = 2;
	vertexIds[31] = 6;
	vertexIds[32] = 7;

	// Top Face 2:
	vertexIds[33] = 2;
	vertexIds[34] = 7;
	vertexIds[35] = 3;
}

std::array<int, 36>& Cube::GetVertexIds() {
	return vertexIds;
}

std::array<Vertex, 8>& Cube::GetVertices() {
	return vertices;
}
