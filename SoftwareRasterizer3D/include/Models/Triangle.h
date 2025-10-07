#pragma once
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Graphics/VertexPostClip.h"

class Triangle {
public:
	Triangle() = default;
	Triangle(VertexPostClip A, VertexPostClip B, VertexPostClip C, uint32_t colour = 0xFF0000FF); // Will change later (bad way to handle colour as well).
	//Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32_t colour = 0xFF0000FF); // WIll change later

	const VertexPostClip& GetVertexA() const;
	const VertexPostClip& GetVertexB() const;
	const VertexPostClip& GetVertexC() const;
	uint32_t GetColour() const;
	Vec3<float> GetFaceNormal() const;
	Vec3<float> ComputeFaceNormal();
	
	void ViewportTransform(int width, int height);

private:
	// Switch to storing indices to a vertex buffer for our triangle instead of the vertex itself!!!!! IMPORTANT
	// helps when we are sharing vertices and more memory efficient. !!! WILL DO AFTER I MAKE A CUBE MODEL!!!
	VertexPostClip A;
	VertexPostClip B;
	VertexPostClip C;

	uint32_t triColour; // colour class instead?
	Vec3<float> faceNormal;
	// maybe interpolate per-vertex normals
	// maybe edge vectors pre-calc?
	
	// GetArea*( helpful for barycentric weights, physics.
	// IsFrontFacing(viewDir) backface culling check
	// containsPoint(p) point-in-triuangle test for picking/collision pineda
	// interpolate(p) returns barycentric interpolation of attributes at point p.

	//----
	// Material reference (points to textures, shaders, reflectivity etc.)
	// Bounding box/bounding spehere for collision detection or spatial partitioning (BVH, octrees)
	// Plane Equation (Ax + By + Cz + D = 0) Good for intersection tests for Ray casting, collision
};

/*
Vec3<T> GetNormalVec(){
	const T u = pB - pA;
	const T v = pC - pA;

	return Vec3<T>::CrossProduct(u, v);
}

*/