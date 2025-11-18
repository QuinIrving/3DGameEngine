#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const ModelAttributes& MA, const Mat4<float>& V, const Mat4<float>& P) {
	Vec4<float> worldPos = vin * MA.modelMatrix;
	Vec4<float> viewPos = worldPos * V;
	Vec3<float> normal = Vec4<float>(vin.GetNormal(), 0.f) * MA.modelMatrix.GetNormalMatrix();//.GetTranspose(); // need to add in getting the inverse here of the matrix.
	normal = normal.GetNormalized();

	VertexOut v{ viewPos * P, worldPos, viewPos, normal, vin.GetUV()};
	return v;
}