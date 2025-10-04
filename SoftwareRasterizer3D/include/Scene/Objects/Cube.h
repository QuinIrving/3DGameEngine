#pragma once
#include "Models/CubeMesh.h"
#include "Math/MatrixVectorOps.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"

class Cube {
public:
	Cube() = default;
	Cube(float size);

	/* [Preferrably use quaternions whenever I get those done and understood]
	Rot (x, y, z) (or by matrix)
	RotX,
	RotY,
	RotZ,
	-----
	Scale(x,y,z) // can become rectangle technically (or by matrix
	ScaleX
	ScaleY
	ScaleZ
	----
	Translate (x,y,z), or by matrix
	TranslateX
	TranslateY
	TranslateZ
	----
	Translation * Rotation * Scale.

	Then generate a model matrix.


	*/
	// Want an ECS so need just making this as a placeholder for now to test my work until refactor time again
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);

	Mat4<float> GetModelMatrix() const;
	std::vector<uint32_t>& GetVertexIds();
	std::vector<VertexIn>& GetVertices();

private:
	Vec3<float> position;
	Vec3<float> rotation;
	Vec3<float> scale{ .5, .5, .5 };
	CubeMesh cube;
};
// Model matrix