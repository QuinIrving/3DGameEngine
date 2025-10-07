#include "Scene/Objects/Cube.h"

Cube::Cube(float size) {
	Scale(size, size, size);
}

void Cube::Rotate(float x, float y, float z) {
	rotation.x =  std::fmodf(rotation.x + x, 360.f);
	rotation.y = std::fmodf(rotation.y + y, 360.f);
	rotation.z = std::fmodf(rotation.z + z, 360.f);
}

void Cube::Scale(float x, float y, float z) {
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void Cube::Translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

Mat4<float> Cube::GetModelMatrix() const {
	return Mat4<float>::Scale(scale) * Mat4<float>::Rotate(rotation) * Mat4<float>::Translate(position);
}

std::vector<uint32_t>& Cube::GetVertexIds() {
	return cube.GetVertexIds();
}

std::vector<VertexIn>& Cube::GetVertices() {
	return cube.GetVertices();
}