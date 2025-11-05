#include "Scene/Objects/UnfoldedCube.h"

UnfoldedCube::UnfoldedCube(float size) {
	Scale(size, size, size);
}

void UnfoldedCube::Rotate(float x, float y, float z) {
	rotation.x = std::fmodf(rotation.x + x, 360.f);
	rotation.y = std::fmodf(rotation.y + y, 360.f);
	rotation.z = std::fmodf(rotation.z + z, 360.f);
}

void UnfoldedCube::Scale(float x, float y, float z) {
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void UnfoldedCube::Translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

ModelAttributes UnfoldedCube::GetModelAttributes() {
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

std::vector<uint32_t>& UnfoldedCube::GetVertexIds() {
	return cube.GetVertexIds();
}

std::vector<VertexIn>& UnfoldedCube::GetVertices() {
	return cube.GetVertices();
}