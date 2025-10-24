#include "Models/Triangle.h"

Triangle::Triangle(VertexPostClip A, VertexPostClip B, VertexPostClip C, Vec3<float> normal, Vec4<float> colour)
	: A(A), B(B), C(C), faceNormal(normal), triColour(colour)
{
	//ComputeFaceNormal();
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;*/
}

/*Triangle::Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32_t colour)
	: triColour(colour)
{
	A = VertexPostClip(x0, y0, z0);
	B = VertexPostClip(x1, y1, z1);
	C = VertexPostClip(x2, y2, z2);
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;
	* /
}*/

const VertexPostClip& Triangle::GetVertexA() const {
	return A;
}

const VertexPostClip& Triangle::GetVertexB() const {
	return B;
}

const VertexPostClip& Triangle::GetVertexC() const {
	return C;
}

Vec4<float> Triangle::GetColour() const {
	return triColour;
}

Vec3<float> Triangle::ComputeFaceNormal(const Vec3<float>& A, const Vec3<float>& B, const Vec3<float>& C) {
	const Vec3<float> u = C - A;
	const Vec3<float> v = B - A;

	return Vec3<float>::CrossProduct(u, v).GetNormalized();
}


Vec3<float> Triangle::GetFaceNormal() const {
	return faceNormal;
}

void Triangle::ViewportTransform(int width, int height) {
	A.ViewportTransform(width, height);
	B.ViewportTransform(width, height);
	C.ViewportTransform(width, height);
}
