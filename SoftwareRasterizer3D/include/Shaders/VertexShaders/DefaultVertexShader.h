#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const Mat4<float>& M, const Mat4<float>& V, const Mat4<float>& P) {
	return VertexOut(vin * M * V * P);
}