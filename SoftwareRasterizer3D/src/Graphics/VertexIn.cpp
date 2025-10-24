#include "Graphics/VertexIn.h"

Vec4<float> VertexIn::operator*(const Mat4<float>& rhs) const {
	Vec4<float> homogenousPos = Vec4<float>(position) * rhs;
	return homogenousPos;
}

const Vec3<float>& VertexIn::GetPosition() const {
	return position;
}

const Vec4<float>& VertexIn::GetColour() const {
	return colour;
}

const Vec3<float>& VertexIn::GetNormal() const {
	return normal;
}

const Vec2<float>& VertexIn::GetUV() const {
	return UV;
}

void VertexIn::SetNormal(Vec3<float> norm) {
	normal = norm;
} 

void VertexIn::SetColour(int r, int g, int b, int a) {
	colour.x = r / 255.f;
	colour.y = g / 255.f;
	colour.z = b / 255.f;
	colour.w = a / 255.f;
}