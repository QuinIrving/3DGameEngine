#include "Models/CubeMesh.h"
#include "Models/Triangle.h"

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
	vertexIds.push_back(3);
	vertexIds.push_back(2);

	// Front Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(1);
	vertexIds.push_back(3);

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
	vertexIds.push_back(7);
	vertexIds.push_back(3);

	// Right Face 2:
	vertexIds.push_back(1);
	vertexIds.push_back(5);
	vertexIds.push_back(7);

	// Left Face 1:
	vertexIds.push_back(0);
	vertexIds.push_back(2);
	vertexIds.push_back(6);

	// Left Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(6);
	vertexIds.push_back(4);

	// Top Face 1:
	vertexIds.push_back(0);
	vertexIds.push_back(4);
	vertexIds.push_back(5);

	// Top Face 2:
	vertexIds.push_back(0);
	vertexIds.push_back(5);
	vertexIds.push_back(1);

	// Bottom Face 1:
	vertexIds.push_back(2);
	vertexIds.push_back(7);
	vertexIds.push_back(6);

	// Bottom Face 2:
	vertexIds.push_back(2);
	vertexIds.push_back(3);
	vertexIds.push_back(7);

	SetVertexNormals();
}

std::vector<uint32_t>& CubeMesh::GetVertexIds() {
	return vertexIds;
}

std::vector<VertexIn>& CubeMesh::GetVertices() {
	return vertices;
}

void CubeMesh::SetVertexNormals() {
	//vertexIds;
	//vertices;
	std::vector<Vec3<float>> faceNormals;
	faceNormals.resize(12);

	std::vector<Vec3<float>> vertNormals;
	vertNormals.resize(8);

	for (int i = 0; i < vertexIds.size(); i += 3) {
		faceNormals[i / 3] = Triangle::ComputeFaceNormal(vertices[vertexIds[i]].GetPosition(), vertices[vertexIds[i + 1]].GetPosition(), vertices[vertexIds[i + 2]].GetPosition());
		// add in the vertices for us to average them.
		vertNormals[vertexIds[i]] += faceNormals[i / 3];
		vertNormals[vertexIds[i+1]] += faceNormals[i / 3];
		vertNormals[vertexIds[i+2]] += faceNormals[i / 3];
	}

	// average and normalize the vert normals that had each face normal added.
	for (int i = 0; i < vertices.size(); ++i) {
		if (abs(vertNormals[i].x) > 1) {
			vertNormals[i].x /= abs(vertNormals[i].x);
		}

		if (abs(vertNormals[i].y) > 1) {
			vertNormals[i].y /= abs(vertNormals[i].y);
		}

		if (abs(vertNormals[i].z) > 1) {
			vertNormals[i].z /= abs(vertNormals[i].z);
		}
		vertices[i].SetNormal((vertNormals[i]).Normalize());
	}
}