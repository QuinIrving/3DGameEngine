#include "Models/Triangle.h"

Triangle::Triangle(VertexIn A, VertexIn B, VertexIn C, uint32_t colour)
	: A(A), B(B), C(C), triColour(colour)
{
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;*/
}

Triangle::Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32_t colour)
	: triColour(colour)
{
	A = VertexIn(x0, y0, z0);
	B = VertexIn(x1, y1, z1);
	C = VertexIn(x2, y2, z2);
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;
	*/
}

const VertexIn& Triangle::GetVertexA() const {
	return A;
}

const VertexIn& Triangle::GetVertexB() const {
	return B;
}

const VertexIn& Triangle::GetVertexC() const {
	return C;
}

uint32_t Triangle::GetColour() const {
	return triColour;
}

Vec3<float> Triangle::ComputeFaceNormal() {
	const Vec3<float> u = B.GetPosition() - A.GetPosition();
	const Vec3<float> v = C.GetPosition() - A.GetPosition();

	faceNormal = Vec3<float>::CrossProduct(u, v).GetNormalized();
	return faceNormal;
}


Vec3<float> Triangle::GetFaceNormal() const {
	return faceNormal;
}
