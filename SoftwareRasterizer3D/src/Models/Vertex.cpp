#include "Models/Vertex.h"

Vec3<float>& Vertex::GetPosition() {
	return position;
}

Vec4<int>& Vertex::GetColour() {
	return colour;
}

Vec3<float>& Vertex::GetNormal() {
	return normal;
}

Vec2<float>& Vertex::GetUV() {
	return UV;
}

void Vertex::CalculateNormal() {
	// not sure exactly how to implement this.
	// saw something about summing face normals of the faces that share the vertex. and normalize it.
}

void Vertex::SetColour(int r, int g, int b, int a) {
	colour.x = r;
	colour.y = g;
	colour.z = b;
	colour.w = a;
}