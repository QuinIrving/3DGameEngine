#include "Models/QuadMesh.h"

QuadMesh::QuadMesh(float width, float height) {
	vertices.reserve(4);
	vertexIds.reserve(6);

	//VertexIn v1 = VertexIn(0, 0, 1, { 0, 0, 1 }, { 0, 0 });
	//VertexIn v2 = VertexIn(width, 0, 1, {0, 0, 1}, {1, 0});
	//VertexIn v3 = VertexIn(0, height, 1, { 0, 0, 1 }, { 0, 1 });
	//VertexIn v4 = VertexIn(width, height, 1, { 0, 0, 1 }, { 1, 1 });
	vertices.push_back({ 0, 0, 1, {0, 0, 1}, {0, 0} });
	vertices.push_back({width, 0, 1, {0, 0, 1}, {1, 0} });
	vertices.push_back({0, height, 1, {0, 0, 1}, {0, 1} });
	vertices.push_back({width, height, 1, {0, 0, 1}, {1, 1} });

	vertexIds = { 0, 2, 1, 1, 2, 3 };

	for (int i = 0; i < vertexIds.size(); i += 3) {
		Vec3<float> edge1 = vertices[vertexIds[i + 1]].GetPosition() - vertices[vertexIds[i]].GetPosition();
		Vec3<float> edge2 = vertices[vertexIds[i + 2]].GetPosition() - vertices[vertexIds[i]].GetPosition();

		Vec2<float> dUV1 = vertices[vertexIds[i + 1]].GetUV() - vertices[vertexIds[i]].GetUV();
		Vec2<float> dUV2 = vertices[vertexIds[i + 2]].GetUV() - vertices[vertexIds[i]].GetUV();

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

		Vec3<float> tangent = (edge1 * dUV2.y - edge2 * dUV1.y) * f;
		tangent = tangent.GetNormalized();
		Vec3<float> bitangent = (edge1 * -dUV2.x + edge2 * dUV1.x) * f;
		bitangent = bitangent.GetNormalized();
		
		vertices[vertexIds[i]].SetTangent(tangent);
		vertices[vertexIds[i]].SetTangentW((vertices[vertexIds[i]].GetNormal().CrossProduct(tangent).DotProduct(bitangent)) < 0.0f ? -1.0f : 1.0f );

		vertices[vertexIds[i + 1]].SetTangent(tangent);
		vertices[vertexIds[i + 1]].SetTangentW((vertices[vertexIds[i + 1]].GetNormal().CrossProduct(tangent).DotProduct(bitangent)) < 0.0f ? -1.0f : 1.0f);

		vertices[vertexIds[i + 2]].SetTangent(tangent);
		vertices[vertexIds[i + 2]].SetTangentW((vertices[vertexIds[i + 2]].GetNormal().CrossProduct(tangent).DotProduct(bitangent)) < 0.0f ? -1.0f : 1.0f);
	}
}