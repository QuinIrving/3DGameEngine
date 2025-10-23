#pragma once
#include "Math/Vec4.h"
#include "Math/Vec3.h"
#include "Math/Vec2.h"

class VertexPostClip {
public:
	VertexPostClip() = default;
	VertexPostClip(Vec3<float> position, float invW, Vec3<float> viewPosition, Vec4<float> colour, Vec3<float> normal, Vec2<float> UV) 
		: position(position), invW(invW), viewPosition(viewPosition), colour(colour), normal(normal), UV(UV) {}

	const Vec3<float>& GetPosition() const;
	const Vec3<float>& GetViewPosition() const;
	const Vec4<float>& GetColour() const;
	const Vec3<float>& GetNormal() const;
	const Vec2<float>& GetUV() const;
	const bool GetInvW() const;

	void ViewportTransform(int width, int height);

private:
	Vec3<float> position;
	Vec3<float> viewPosition;
	float invW;
	Vec4<float> colour;
	Vec3<float> normal;
	Vec2<float> UV;
};