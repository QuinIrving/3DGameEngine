#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const Mat4<float>& M, const Mat4<float>& V, const Mat4<float>& P) {
	//Vec4<float> mPos = vin * M;
	Vec4<float> viewPos = vin * M * V;
	Vec3<float> normal = Vec4<float>(vin.GetNormal(), 0.f) * M.GetTranspose();
	normal = normal.GetNormalized();

	VertexOut v{ viewPos * P, viewPos, normal };
	/*OutputDebugString(std::format(L"---------\npos: ({}, {}, {})\nmodelPos: ({}, {}, {})\nnormalIn: ({}, {}, {})\nnormalOut: ({}, {}, {})\n\n",
		vin.GetPosition().x, vin.GetPosition().y, vin.GetPosition().z,
		mPos.x, mPos.y, mPos.z,
		vin.GetNormal().x, vin.GetNormal().y, vin.GetNormal().z,
		normal.x, normal.y, normal.z).c_str());*/
	return v;
}