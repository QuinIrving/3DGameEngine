#include "Models/Vertex.h"

const Vec3<float>& Vertex::GetPosition() const {
	return position;
}

const Vec4<int>& Vertex::GetColour() const {
	return colour;
}

const Vec3<float>& Vertex::GetNormal() const {
	return normal;
}

const Vec2<float>& Vertex::GetUV() const {
	return UV;
}

const Vec3<float>& Vertex::CalculateNormal() {
	return normal;
	// not sure exactly how to implement this.
	// saw something about summing face normals of the faces that share the vertex. and normalize it.
}

void Vertex::SetColour(int r, int g, int b, int a) {
	colour.x = r;
	colour.y = g;
	colour.z = b;
	colour.w = a;
}