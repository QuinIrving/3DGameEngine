#pragma once
#include "Shaders/ShaderSignatures.h"
#include "Graphics/ModelAttributes.h"
#include <algorithm>

/**********
Custom Addition for testing out flat shading in basic respect.
*******/
#include "Scene/Lights/Delta/DirectionalLight.h"

DirectionalLight directionLight{ {0.1f, -0.25f, -0.7f} };

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
	}

	/**********
	Custom Addition for testing out flat shading in basic respect.
	*******/
	// fancy name just for the diffuse intensity
	float lambertianReflectance = std::max(0.1f, fragIn.faceNormal.DotProduct(-directionLight.direction));
	lambertianReflectance *= directionLight.intensity;
	// don't forget about colour, and looping through later, and better ways to do some of this.

	c *= lambertianReflectance;
	/**
	End of custom addition
	*/

	fragOut.colour = { c.r * 255.f, c.g * 255.f, c.b * 255.f, c.a * 255.f };
	return fragOut;
}