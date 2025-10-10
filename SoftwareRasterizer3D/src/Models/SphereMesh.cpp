#include "Models/SphereMesh.h"
#include "Math/GeneralMath.h"
#include "Math/Quaternion.h"



// 1.f, 16, 8
SphereMesh::SphereMesh(float radius, int numLatDivs, int numLongDivs) 
	: radius(radius), numLatDivs(numLatDivs), numLongDivs(numLongDivs)
{
	Vec4<float> initialVPos{ 0.f, 0.f, radius, 1.f };
	float latAngle = 2.f * PI / numLatDivs;
	float longAngle = PI / numLongDivs;

	vertices.reserve(numLatDivs * numLongDivs);

	// rotate around our x in long angle to get the vertical position.
	// rotate around our y in lat angle to get horizontal position.
	

	// generate our sphere vertices.
	for (int longIdx = 1; longIdx < numLongDivs; ++longIdx) {
		Vec4<float> vLongPos = initialVPos * Quaternion(longAngle * longIdx, 0.f, 0.f).GetRotationMatrix(); // optimizations could be made here
		
		for (int latIdx = 0; latIdx < numLatDivs; ++latIdx) {
			vertices.push_back(VertexIn(vLongPos * Quaternion(0.f, latAngle * latIdx, 0.f).GetRotationMatrix())); // and optimizations here with my quaternion.
		}
	}

	vertexIds.reserve(numLongDivs * numLatDivs * 6); // 2 triangles * (3 vertices) per quads = 6 verts a quad.

	// generate our vertex id's for 2 triangles per quad
	for (int longIdx = 0; longIdx < numLongDivs - 2; ++longIdx) {
		for (int latIdx = 0; latIdx < numLatDivs - 1; ++latIdx) {
			// in a quad we have top left, top right, bottom left, bottomRIght
			int topLeft = (longIdx * numLatDivs) + latIdx;
			int topRight = (longIdx * numLatDivs) + (latIdx + 1);
			int bottomLeft = ((longIdx + 1) * numLatDivs) + latIdx;
			int bottomRight = ((longIdx + 1) * numLatDivs) + (latIdx + 1);

			//CCW Winding: topLeft->bottomLeft->topRight & bottomLeft->bottomRight->topRight
			vertexIds.emplace_back(topLeft); // emplace back seems better when I look it up. No temp obj.
			vertexIds.emplace_back(bottomLeft);
			vertexIds.emplace_back(topRight);

			vertexIds.emplace_back(bottomLeft);
			vertexIds.emplace_back(bottomRight);
			vertexIds.emplace_back(topRight);
		}
	}

}