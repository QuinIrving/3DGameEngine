#include "Graphics/VertexPostClip.h"

const Vec3<float>& VertexPostClip::GetPosition() const {
	return position;
}

const Vec3<float>& VertexPostClip::GetWorldPosition() const {
	return worldPosition;
}

const Vec3<float>& VertexPostClip::GetViewPosition() const {
	return viewPosition;
}

const Vec4<float>& VertexPostClip::GetColour() const {
	return colour;
}

const Vec3<float>& VertexPostClip::GetNormal() const {
	return normal;
}

const Vec2<float>& VertexPostClip::GetUV() const {
	return UV;
}

const float VertexPostClip::GetInvW() const {
	return invW;
}

const Vec3<float>& VertexPostClip::GetTangent() const {
	return tangent;
}

const Vec3<float>& VertexPostClip::GetBitangent() const {
	return bitangent;
}

void VertexPostClip::ViewportTransform(int width, int height) {
	// may want to check z here, if it's -1,1 it means I need to change my projection matrix.
	position.x = (position.x + 1.f) * 0.5f * width;
	position.y = (1.f - position.y) * 0.5f * height;
}