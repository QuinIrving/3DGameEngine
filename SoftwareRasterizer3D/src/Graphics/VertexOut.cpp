#include "Graphics/VertexOut.h"

Vec4<float> VertexOut::operator*(const Mat4<float>& rhs) const {
	return position * rhs;
}

const Vec4<float>& VertexOut::GetPosition() const {
	return position;
}

const Vec3<float>& VertexOut::GetWorldPosition() const {
	return worldPosition;
}

const Vec3<float>& VertexOut::GetViewPosition() const {
	return viewPosition;
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

const Vec3<float>& VertexOut::GetTangent() const {
	return tangent;
}

const Vec3<float>& VertexOut::GetBitangent() const {
	return bitangent;
}

/*const Vec3<float>& VertexOut::CalculateNormal() {
	return normal;
	// not sure exactly how to implement this.
	// saw something about summing face normals of the faces that share the vertex. and normalize it.
}*/

void VertexOut::SetColour(int r, int g, int b, int a) {
	colour.x = r / 255.f;
	colour.y = g / 255.f;
	colour.z = b / 255.f;
	colour.w = a / 255.f;
}

bool VertexOut::IsTriangleInFrustum(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3) {
	const Vec4<float>& pos1 = v1.GetPosition();
	const Vec4<float>& pos2 = v2.GetPosition();
	const Vec4<float>& pos3 = v3.GetPosition();

	// check against all planes.
	bool vertInLeftPlane = v1.IsInFrustumPlane(TestPlane::LEFT) || v2.IsInFrustumPlane(TestPlane::LEFT) || v3.IsInFrustumPlane(TestPlane::LEFT);
	bool vertInRightPlane = v1.IsInFrustumPlane(TestPlane::RIGHT) || v2.IsInFrustumPlane(TestPlane::RIGHT) || v3.IsInFrustumPlane(TestPlane::RIGHT);
	bool vertInTopPlane = v1.IsInFrustumPlane(TestPlane::TOP) || v2.IsInFrustumPlane(TestPlane::TOP) || v3.IsInFrustumPlane(TestPlane::TOP);
	bool vertInBottomPlane = v1.IsInFrustumPlane(TestPlane::BOTTOM) || v2.IsInFrustumPlane(TestPlane::BOTTOM) || v3.IsInFrustumPlane(TestPlane::BOTTOM);
	bool vertInFrontPlane = v1.IsInFrustumPlane(TestPlane::FRONT) || v2.IsInFrustumPlane(TestPlane::FRONT) || v3.IsInFrustumPlane(TestPlane::FRONT);
	bool vertInBackPlane = v1.IsInFrustumPlane(TestPlane::BACK) || v2.IsInFrustumPlane(TestPlane::BACK) || v3.IsInFrustumPlane(TestPlane::BACK);

	return vertInLeftPlane && vertInRightPlane && vertInTopPlane && vertInBottomPlane && vertInFrontPlane && vertInBackPlane;
}

bool VertexOut::IsInFrustumPlane(TestPlane plane) const {
	
	switch (plane) {
	case TestPlane::LEFT:
		return position.x >= -position.w;
		break;
	case TestPlane::RIGHT:
		return position.x <= position.w;
		break;
	case TestPlane::TOP:
		return position.y >= -position.w;
		break;
	case TestPlane::BOTTOM:
		return position.y <= position.w;
		break;
	case TestPlane::FRONT:
		return position.z >= 0;
		break;
	case TestPlane::BACK:
		return position.z <= position.w;
		break;
	default:
		return false;
	}
}

bool VertexOut::IsNotInNearFrustum() const {
	return position.z < 0;
}

VertexPostClip VertexOut::PerspectiveDivide() const {
	float pX = position.x / position.w;
	float pY = position.y / position.w;
	float pZ = position.z / position.w;

	float invW = 1 / position.w;

	return VertexPostClip(Vec3<float>(pX, pY, pZ), invW, worldPosition, viewPosition, colour, normal, UV, tangent, bitangent); // pass in all of the calculated normal, texcord, colour, x,y,z, invW, and other vertex attributes.
}