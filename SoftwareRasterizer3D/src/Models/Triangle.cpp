#include "Models/Triangle.h"
#include <Windows.h>
#include <format>

Triangle::Triangle(Vertex A, Vertex B, Vertex C, uint32_t colour) 
	: A(A), B(B), C(C), triColour(colour)
{
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;*/
}

Triangle::Triangle(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, uint32_t colour) 
	: triColour(colour)
{
	A = Vertex(x0, y0, z0);
	B = Vertex(x1, y1, z1);
	C = Vertex(x2, y2, z2);
	/*
	int r = ;
	int g = ;
	int b = ;
	int a = ;
	*/
}

Vec3<float> Triangle::ComputeFaceNormal() {
	const Vec3<float> u = B.GetPosition() - A.GetPosition();
	const Vec3<float> v = C.GetPosition() - A.GetPosition();

	Vec3<float> test = Vec3<float>::CrossProduct(u, v);
	OutputDebugString(std::format(L"\nNon-normalized. X: {}, Y: {}, Z: {}, MAG: {}\n\n", test.x, test.y, test.z, test.GetMagnitude()).c_str());

	faceNormal = Vec3<float>::CrossProduct(u, v).GetNormalized();
	return faceNormal;
}


Vec3<float> Triangle::GetFaceNormal() const {
	return faceNormal;
}
