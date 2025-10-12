#include "Graphics/VertexOut.h"

Vec4<float> VertexOut::operator*(const Mat4<float>& rhs) const {
	return position * rhs;
}

const Vec4<float>& VertexOut::GetPosition() const {
	return position;
}

const Vec4<float>& VertexOut::GetColour() const {
	return colour;
}

const Vec3<float>& VertexOut::GetNormal() const {
	return normal;
}

const Vec2<float>& VertexOut::GetUV() const {
	return UV;
}

const Vec3<float>& VertexOut::CalculateNormal() {
	return normal;
	// not sure exactly how to implement this.
	// saw something about summing face normals of the faces that share the vertex. and normalize it.
}

void VertexOut::SetColour(int r, int g, int b, int a) {
	colour.x = r / 255.f;
	colour.y = g / 255.f;
	colour.z = b / 255.f;
	colour.w = a / 255.f;
}

bool VertexOut::IsInFrustum() const {
	return (position.x >= -position.w && position.x <= position.w)
		&& (position.y >= -position.w && position.y <= position.w)
		&& (position.z >= 0 && position.z <= position.w); // since we map z to 0 to 1.
}

bool VertexOut::IsNotInNearFrustum() const {
	return position.z < 0;
}

VertexPostClip VertexOut::PerspectiveDivide() const {
	float pX = position.x / position.w;
	float pY = position.y / position.w;
	float pZ = position.z / position.w;

	float invW = 1 / position.w;

	return VertexPostClip(Vec3<float>(pX, pY, pZ), invW, colour, normal, UV); // pass in all of the calculated normal, texcord, colour, x,y,z, invW, and other vertex attributes.
}