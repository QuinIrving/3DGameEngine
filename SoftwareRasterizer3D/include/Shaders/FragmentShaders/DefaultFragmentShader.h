#pragma once
#include "Shaders/ShaderSignatures.h"

FragmentOut DefaultFragmentShader(const FragmentIn& fragIn) {
	FragmentOut fragOut;
	fragOut.colour = fragIn.colour;
	return fragOut;
}