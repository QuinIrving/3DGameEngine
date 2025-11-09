#pragma once
#include "Shaders/ShaderSignatures.h"

FragmentOut DefaultFragmentShader(const FragmentIn& fragIn, const Material& material) {
	FragmentOut fragOut;
	Colour c;
	// no textures, so just handle base colour.
	if (!material.albedoTexture || *material.albedoTexture == NULL) {
		c = material.baseColour;
	}
	else {
		// some other flows here needed to add in as we add in PBR.
		switch (material.samplingMode) {
		case TextureSamplingMode::Nearest:
			c = (*material.albedoTexture)->SampleNearest(fragIn.uv.x, fragIn.uv.y);
			break;
		case TextureSamplingMode::Bilinear:
			c = (*material.albedoTexture)->SampleBilinear(fragIn.uv.x, fragIn.uv.y);
			break;
		default:
			c = { 1.f, 1.f, 1.f, 1.f }; // should be base colour instead.
		}
	}

	fragOut.colour = { c.r * 255.f, c.g * 255.f, c.b * 255.f, c.a * 255.f };
	return fragOut;
}