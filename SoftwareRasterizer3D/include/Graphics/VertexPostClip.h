#pragma once
#include "Math/Vec4.h"
#include "Math/Vec3.h"
#include "Math/Vec2.h"

class VertexPostClip {
public:
	VertexPostClip() = default;
	VertexPostClip(Vec3<float> position, float invW, Vec3<float> worldPosition, Vec3<float> viewPosition, Vec4<float> colour, Vec3<float> normal, Vec2<float> UV) 
		: position(position), invW(invW), worldPosition(worldPosition), viewPosition(viewPosition), colour(colour), normal(normal), UV(UV) {};
	VertexPostClip(Vec3<float> position, float invW, Vec3<float> worldPosition, Vec3<float> viewPosition, Vec4<float> colour, Vec3<float> normal, Vec2<float> UV, Vec3<float> tangent, Vec3<float> bitangent)
		: position(position), invW(invW), worldPosition(worldPosition), viewPosition(viewPosition), colour(colour), normal(normal), UV(UV), tangent(tangent), bitangent(bitangent) {};

	const Vec3<float>& GetPosition() const;
	const Vec3<float>& GetWorldPosition() const;
	const Vec3<float>& GetViewPosition() const;
	const Vec4<float>& GetColour() const;
	const Vec3<float>& GetNormal() const;
	const Vec2<float>& GetUV() const;
	const float GetInvW() const;
	const Vec3<float>& GetTangent() const;
	const Vec3<float>& GetBitangent() const;

	void ViewportTransform(int width, int height);

private:
	Vec3<float> position;
	Vec3<float> worldPosition;
	Vec3<float> viewPosition;
	float invW;
	Vec4<float> colour;
	Vec3<float> normal;
	Vec2<float> UV;
	Vec3<float> tangent = { 0, 0, 0 };
	Vec3<float> bitangent = { 0, 0, 0 };
};