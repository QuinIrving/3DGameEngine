#include "Models/SphereMesh.h"
#include "Math/GeneralMath.h"
#include "Math/Quaternion.h"



// 1.f, 16, 8
SphereMesh::SphereMesh(float radius, int numLatDivs, int numLongDivs) 
	: radius(radius), numLatDivs(numLatDivs), numLongDivs(numLongDivs)
{

	vertices.reserve(numLatDivs * numLongDivs + 2);

	// rotate around our x in long angle to get the vertical position.
	// rotate around our y in lat angle to get horizontal position.
	
	// generate our sphere vertices.
	for (int longIdx = 0; longIdx <= numLongDivs; ++longIdx) {

		float phi = longIdx * PI / numLongDivs;
		float sinPhi = std::sinf(phi);
		float cosPhi = std::cosf(phi);

		for (int latIdx = 0; latIdx < numLatDivs; ++latIdx) {
			float theta = latIdx * 2.0f * PI / numLatDivs;
			float sinTheta = std::sin(theta);
			float cosTheta = std::cos(theta);

			float x = radius * sinPhi * cosTheta;
			float y = radius * cosPhi;
			float z = radius * sinPhi * sinTheta;

			Vec4<float> pos = Vec4<float>(x, y, z, 1.f);

			// get normal.
			Vec3<float> normal = Vec3<float>(x, y, z);

			float u = static_cast<float>(latIdx) / numLatDivs;
			float v = static_cast<float>(longIdx) / numLongDivs;

			vertices.emplace_back(VertexIn(pos, normal, Vec2<float>(u, v)));
		}
	}

	vertexIds.reserve(numLongDivs * numLatDivs * 6); // 6 verts per quad.

	for (int longIdx = 0; longIdx < numLongDivs; ++longIdx) {
		for (int latIdx = 0; latIdx < numLatDivs; ++latIdx) {
			int topLeft = (longIdx * numLatDivs) + latIdx;
			int topRight = (longIdx * numLatDivs) + (latIdx + 1) % numLatDivs;
			int bottomLeft = ((longIdx + 1) * numLatDivs) + latIdx;
			int bottomRight = ((longIdx + 1) * numLatDivs) + (latIdx + 1) % numLatDivs;

			//CCW Winding: topLeft->bottomLeft->topRight
			vertexIds.emplace_back(topLeft); // emplace back seems better when I look it up. No temp obj.
			vertexIds.emplace_back(bottomLeft);
			vertexIds.emplace_back(topRight);

			// CCW Winding: bottomLeft->bottomRight->topRight
			vertexIds.emplace_back(bottomLeft);
			vertexIds.emplace_back(bottomRight);
			vertexIds.emplace_back(topRight);
		}
	}
}