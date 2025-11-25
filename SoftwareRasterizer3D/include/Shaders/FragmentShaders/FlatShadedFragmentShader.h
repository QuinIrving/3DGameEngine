#pragma once
#include "Shaders/ShaderSignatures.h"
#include "Graphics/ModelAttributes.h"
#include <algorithm>

/**********
Custom Addition for testing out flat shading in basic respect.
*******/
#include "Scene/Lights/Delta/DirectionalLight.h"

DirectionalLight directionLight{ {0.1f, -0.25f, -0.7f}, 10.f};

/**
End of custom addition
*/

FragmentOut FlatShadedFragmentShader(const FragmentIn& fragIn, const Material& material) {
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

	/**********
	Custom Addition for testing out flat shading in basic respect.
	*******/
	// fancy name just for the diffuse intensity
	float lambertianReflectance = std::max(0.0f, fragIn.faceWorldNormal.DotProduct(-directionLight.direction));
	lambertianReflectance *= directionLight.intensity;
	// don't forget about colour, and looping through later, and better ways to do some of this.

	c *= lambertianReflectance;
	/**
	End of custom addition
	*/

	fragOut.colour = { c.r, c.g, c.b, c.a };
	return fragOut;
}