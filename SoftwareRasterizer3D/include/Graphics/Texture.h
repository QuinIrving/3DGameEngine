#pragma once
#include <vector>
#include "Colour.h"
#include <string>

struct Texture {
	int width;
	int height;
	std::vector<Colour> texels;

	// can add in option to switch between nearest neighbour and stuff like bilinear filtering.
	Colour Sample(float u, float v) const;

	Texture(const std::string& texturePath);
};