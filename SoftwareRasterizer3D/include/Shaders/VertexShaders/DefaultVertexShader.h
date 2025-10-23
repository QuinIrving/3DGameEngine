#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const Mat4<float>& M, const Mat4<float>& V, const Mat4<float>& P) {
	Vec4<float> viewPos = vin * M * V;
	VertexOut v{ viewPos * P, viewPos, {} };
	
	return v;
}