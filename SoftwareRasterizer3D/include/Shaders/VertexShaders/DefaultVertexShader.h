#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const Mat4<float>& M, const Mat4<float>& V, const Mat4<float>& P) {
	Vec4<float> viewPos = vin * M * V;
	Vec3<float> normal = Vec4<float>(vin.GetNormal(), 0.f) * M.GetTranspose();
	normal = normal.GetNormalized();

	VertexOut v{ viewPos * P, viewPos, normal, vin.GetUV()};
	return v;
}