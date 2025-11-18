#pragma once
#include "Math/Vec4.h"
#include "Math/Vec3.h"
#include "Math/Vec2.h"

struct FragmentIn {
public: // defaults but for clarity.
	int x, y;
	float z; //interpolated z
	// all attributes should be interpolated (perspective-correct)
	Vec3<float> pointNormal; // should be normalized after interpolation
	Vec3<float> faceNormal;
	Vec3<float> faceWorldNormal;
	// light value for gouraud shading????
	Vec4<float> colour{ 1, 1, 1, 1 }; // vertex colour (could be blended)
	Vec2<float> uv; // Texture coordinates. All maps seem to use it
	Vec3<float> tangent; // for normal mapping
	Vec3<float> bitangent; // for normal mapping

	FragmentIn() = default;
	FragmentIn(int x, int y, float z) : x(x), y(y), z(z) {}
	FragmentIn(int x, int y, float z, const Vec4<float>& colour) : x(x), y(y), z(z), colour(colour) {}
	FragmentIn(int x, int y, float z, const Vec3<float>& vertNormal) : x(x), y(y), z(z), pointNormal(vertNormal.GetNormalized()) {}
	FragmentIn(int x, int y, float z, const Vec3<float>& vertNormal, const Vec3<float>& faceNormal, const Vec3<float> worldNormal, const Vec4<float>& colour) : x(x), y(y), z(z), pointNormal(vertNormal.GetNormalized()), faceNormal(faceNormal), faceWorldNormal(worldNormal), colour(colour) {}
	FragmentIn(int x, int y, float z, const Vec3<float>& vertNormal, const Vec3<float>& faceNormal, const Vec3<float> worldNormal, const Vec4<float>& colour, const Vec2<float>& uv, const Vec3<float>& tangent, const Vec3<float>& bitangent)
		: x(x), y(y), z(z), pointNormal(vertNormal.GetNormalized()), faceNormal(faceNormal), faceWorldNormal(worldNormal), colour(colour), uv(uv), tangent(tangent), bitangent(bitangent) {}

};