#pragma once
#include <vector>
#include "Graphics/VertexIn.h"
// Should be our generic mesh, so it can also be used by obj loaders. May need it to include normals? idk, look into a generic Object then as well?

class QuadMesh {
public:
	QuadMesh(float width = 1.f, float height = 1.f);

	std::vector<uint32_t>& GetVertexIds() { return vertexIds; }
	std::vector<VertexIn>& GetVertices() { return vertices; }

	void SetVertexNormals();
private:
	std::vector<VertexIn> vertices;
	std::vector<uint32_t> vertexIds;
};