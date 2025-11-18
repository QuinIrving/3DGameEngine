#include "Scene/Objects/Object.h"

Object::Object(float size, std::vector<VertexIn> vertices, std::vector<uint32_t> vertexIds) {
	Scale(size, size, size);
	mesh = Mesh(vertices, vertexIds);
}

void Object::Rotate(float x, float y, float z) {
	rotation.x = std::fmodf(rotation.x + x, 360.f);
	rotation.y = std::fmodf(rotation.y + y, 360.f);
	rotation.z = std::fmodf(rotation.z + z, 360.f);
}

void Object::Scale(float x, float y, float z) {
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void Object::Translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

ModelAttributes Object::GetModelAttributes() {
	if (rotation.x != 0.f || rotation.y != 0.f || rotation.z != 0.f) {
		delta *= Quaternion(rotation.x, rotation.y, rotation.z);
		delta.Normalize();

		rotation.x = 0;
		rotation.y = 0;
		rotation.z = 0;
	}

	ModelAttributes ma;
	ma.material = material;
	ma.modelMatrix = Mat4<float>::Scale(scale) * delta.GetRotationMatrix() * Mat4<float>::Translate(position);
	return ma;
}

std::vector<uint32_t>& Object::GetVertexIds() {
	return mesh.GetVertexIds();
}

std::vector<VertexIn>& Object::GetVertices() {
	return mesh.GetVertices();
}