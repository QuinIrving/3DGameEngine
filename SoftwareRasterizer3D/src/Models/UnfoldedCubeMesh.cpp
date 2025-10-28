#include "Models/UnfoldedCubeMesh.h"
#include "Models/Triangle.h"
#include <ranges>

// Look into std::span for using std::array instead of vertex. See if it will help with efficiency.
UnfoldedCubeMesh::UnfoldedCubeMesh() {
	vertices.reserve(24);
	vertexIds.reserve(36);

	float size = 1.f;

	/*
	For UV:
	EMPTY +Y  EMPTY EMPTY
	-X    -Z  +X    +Z
	EMPTY -Y  EMPTY EMPTY
	*/

	// TL: 0,0
	// BR: 1,1

	float faceWidth = 1.f / 4.f;
	float faceHeight = 1.f / 3.f;

	auto makeUV = [&](float col, float row, float u, float v) {
		return Vec2<float>(col * faceWidth + u * faceWidth, row * faceHeight + v * faceHeight);
	};
	
	// Front Face 1: 032
	vertices.push_back({ -size, size, size, {}, makeUV(3, 1, 0, 1)});
	vertices.push_back({ size,  -size, size, {}, makeUV(3, 1, 1, 0) });
	vertices.push_back({ -size,  -size, size, {}, makeUV(3, 1, 0, 0) });

	// Front Face 2: 013
	vertices.push_back({ -size, size, size, {}, makeUV(3, 1, 0, 1) });
	vertices.push_back({ size, size, size, {}, makeUV(3, 1, 1, 1) });
	vertices.push_back({ size,  -size, size, {}, makeUV(3, 1, 1, 0) });

	// Back Face 1: 467
	vertices.push_back({ -size, size,  -size, {}, makeUV(1, 1, 1, 1) });
	vertices.push_back({ -size,  -size,  -size, {}, makeUV(1, 1, 1, 0) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(1, 1, 0, 0) });
	
	// Back Face 2: 475
	vertices.push_back({ -size, size,  -size, {}, makeUV(1, 1, 1, 1) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(1, 1, 0, 0) });
	vertices.push_back({ size, size,  -size, {}, makeUV(1, 1, 0, 1) });

	// Right Face 1: 173
	vertices.push_back({ size, size, size, {}, makeUV(2, 1, 0, 1) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(2, 1, 1, 0) });
	vertices.push_back({ size,  -size, size, {}, makeUV(2, 1, 0, 0) });

	// Right Face 2: 157
	vertices.push_back({ size, size, size, {}, makeUV(2, 1, 0, 1) });
	vertices.push_back({ size, size,  -size, {}, makeUV(2, 1, 1, 1) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(2, 1, 1, 0) });

	// Left Face 1: 026
	vertices.push_back({ -size, size, size, {}, makeUV(0, 1, 1, 1) });
	vertices.push_back({ -size,  -size, size, {}, makeUV(0, 1, 1, 0) });
	vertices.push_back({ -size,  -size,  -size, {}, makeUV(0, 1, 0, 0) });

	// Left Face 2: 064
	vertices.push_back({ -size, size, size, {}, makeUV(0, 1, 1, 1) });
	vertices.push_back({ -size,  -size,  -size, {}, makeUV(0, 1, 0, 0) });
	vertices.push_back({ -size, size,  -size, {}, makeUV(0, 1, 0, 1) });

	// Top Face 1: 045
	vertices.push_back({ -size, size, size, {}, makeUV(1, 0, 0, 0) });
	vertices.push_back({ -size, size,  -size, {}, makeUV(1, 0, 0, 1) });
	vertices.push_back({ size, size,  -size, {}, makeUV(1, 0, 1, 1) });

	// Top Face 2: 051
	vertices.push_back({ -size, size, size, {}, makeUV(1, 0, 0, 0) });
	vertices.push_back({ size, size,  -size, {}, makeUV(1, 0, 1, 1) });
	vertices.push_back({ size, size, size, {}, makeUV(1, 0, 1, 0) });


	// Bottom Face 1: 276
	vertices.push_back({ -size,  -size, size, {}, makeUV(1, 2, 0, 0) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(1, 2, 1, 1) });
	vertices.push_back({ -size,  -size,  -size, {}, makeUV(1, 2, 0, 1) });

	// Bottom Face 2: 237
	vertices.push_back({ -size,  -size, size, {}, makeUV(1, 2, 0, 0) });
	vertices.push_back({ size,  -size, size, {}, makeUV(1, 2, 1, 0) });
	vertices.push_back({ size,  -size,  -size, {}, makeUV(1, 2, 1, 1) });


	auto v = std::views::iota(0, 36);
	vertexIds = std::vector<uint32_t>(v.begin(), v.end());

	SetVertexNormals();
}

std::vector<uint32_t>& UnfoldedCubeMesh::GetVertexIds() {
	return vertexIds;
}

std::vector<VertexIn>& UnfoldedCubeMesh::GetVertices() {
	return vertices;
}

void UnfoldedCubeMesh::SetVertexNormals() {
	for (int i = 0; i < vertexIds.size(); i += 3) {
		Vec3<float> faceNormal = Triangle::ComputeFaceNormal(vertices[vertexIds[i]].GetPosition(), vertices[vertexIds[i + 1]].GetPosition(), vertices[vertexIds[i + 2]].GetPosition());

		vertices[vertexIds[i]].SetNormal(faceNormal);
		vertices[vertexIds[i + 1]].SetNormal(faceNormal);
		vertices[vertexIds[i + 2]].SetNormal(faceNormal);
	}
}