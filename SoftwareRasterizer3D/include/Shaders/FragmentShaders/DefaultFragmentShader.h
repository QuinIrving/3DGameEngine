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

		c = Colour(c.GetVectorizedValues() ^ GAMMA_LINEAR, c.a);
	}

	Vec3<float> rgb = c.GetVectorizedValues();
	// reinhard tone mapping
	rgb = rgb / (rgb + Vec3<float>(1.f, 1.f, 1.f));

	fragOut.colour = { rgb.x, rgb.y, rgb.z, c.a };
	return fragOut;
}