#pragma once
#include "Shaders/ShaderSignatures.h"

VertexOut DefaultVertexShader(const VertexIn& vin, const ModelAttributes& MA, const Mat4<float>& V, const Mat4<float>& P) {
	Vec4<float> worldPos = vin * MA.modelMatrix;
	Vec4<float> viewPos = worldPos * V;

	Mat4<float> normalMatrix = MA.modelMatrix.GetNormalMatrix();
	Vec3<float> normal = Vec4<float>(vin.GetNormal(), 0.f) * normalMatrix;
	normal = normal.GetNormalized();
	Vec3<float> tangent = Vec4<float>(vin.GetTangent(), 0.f) * normalMatrix;
	tangent = tangent.GetNormalized();
	
	// Doing Gram-Schmidt orthongonalization for better accuracy
	tangent = (tangent - (normal * Vec3<float>::DotProduct(normal, tangent))).GetNormalized();

	Vec3<float> bitangent = (normal.CrossProduct(tangent) * vin.GetTangentW()).GetNormalized();

	VertexOut v{ viewPos * P, worldPos, viewPos, normal, vin.GetUV(), tangent, bitangent };
	return v;
}