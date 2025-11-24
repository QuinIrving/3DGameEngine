#include "Models/QuadMesh.h"

QuadMesh::QuadMesh(float width, float height) {
	vertices.reserve(4);
	vertexIds.reserve(6);

	VertexIn();

	vertices.push_back({ 0, 0, 1, {0, 0, 1}, {0, 0} });
	vertices.push_back({width, 0, 1, {0, 0, 1}, {1, 0} });
	vertices.push_back({0, height, 1, {0, 0, 1}, {0, 1} });
	vertices.push_back({width, height, 1, {0, 0, 1}, {1, 1} });

	vertexIds = { 0, 2, 1, 1, 2, 3 };
}